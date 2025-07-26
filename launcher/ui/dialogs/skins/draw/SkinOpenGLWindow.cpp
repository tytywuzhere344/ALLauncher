// SPDX-License-Identifier: GPL-3.0-only
/*
 *  ALLauncher - Minecraft Launcher
 *  Copyright (c) 2024 Trial97 <alexandru.tripon97@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ui/dialogs/skins/draw/SkinOpenGLWindow.h"

#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QVector2D>
#include <QVector3D>
#include <QtMath>

#include "minecraft/skins/SkinModel.h"
#include "rainbow.h"
#include "ui/dialogs/skins/draw/BoxGeometry.h"
#include "ui/dialogs/skins/draw/Scene.h"

SkinOpenGLWindow::SkinOpenGLWindow(SkinProvider* parent, QColor color)
    : QOpenGLWindow(), QOpenGLFunctions(), m_baseColor(color), m_parent(parent)
{
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setDepthBufferSize(24);
    setFormat(format);
}

SkinOpenGLWindow::~SkinOpenGLWindow()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    // double check if resources were initialized because they are not
    // initialized together with the object
    if (m_scene) {
        delete m_scene;
    }
    if (m_background) {
        delete m_background;
    }
    if (m_backgroundTexture) {
        if (m_backgroundTexture->isCreated()) {
            m_backgroundTexture->destroy();
        }
        delete m_backgroundTexture;
    }
    if (m_program) {
        if (m_program->isLinked()) {
            m_program->release();
        }
        m_program->removeAllShaders();
        delete m_program;
    }
    doneCurrent();
}

void SkinOpenGLWindow::mousePressEvent(QMouseEvent* e)
{
    // Save mouse press position
    m_mousePosition = QVector2D(e->pos());
    m_isMousePressed = true;
}

void SkinOpenGLWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isMousePressed) {
        int dx = event->position().x() - m_mousePosition.x();
        int dy = event->position().y() - m_mousePosition.y();

        m_yaw += dx * 0.5f;
        m_pitch += dy * 0.5f;

        // Normalize yaw to keep it manageable
        if (m_yaw > 360.0f)
            m_yaw -= 360.0f;
        else if (m_yaw < 0.0f)
            m_yaw += 360.0f;

        m_mousePosition = QVector2D(event->pos());
        update();  // Trigger a repaint
    }
}

void SkinOpenGLWindow::mouseReleaseEvent([[maybe_unused]] QMouseEvent* e)
{
    m_isMousePressed = false;
}

void SkinOpenGLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 1, 1);

    initShaders();

    generateBackgroundTexture(32, 32, 1);

    QImage skin, cape;
    bool slim = false;
    if (m_parent) {
        if (auto s = m_parent->getSelectedSkin()) {
            skin = s->getTexture();
            slim = s->getModel() == SkinModel::SLIM;
            cape = m_parent->capes().value(s->getCapeId(), {});
        }
    }

    m_scene = new opengl::Scene(skin, slim, cape);
    m_background = opengl::BoxGeometry::Plane();
    glEnable(GL_TEXTURE_2D);
}

void SkinOpenGLWindow::initShaders()
{
    m_program = new QOpenGLShaderProgram(this);
    // Compile vertex shader
    if (!m_program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!m_program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!m_program->link())
        close();

    // Bind shader pipeline for use
    if (!m_program->bind())
        close();
}

void SkinOpenGLWindow::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const qreal zNear = .1, zFar = 1000., fov = 45;

    // Reset projection
    m_projection.setToIdentity();

    // Set perspective projection
    m_projection.perspective(fov, aspect, zNear, zFar);
}

void SkinOpenGLWindow::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_program->bind();

    renderBackground();
    // Calculate model view transformation
    QMatrix4x4 matrix;
    float yawRad = qDegreesToRadians(m_yaw);
    float pitchRad = qDegreesToRadians(m_pitch);
    matrix.lookAt(QVector3D(                                       //
                      m_distance * qCos(pitchRad) * qCos(yawRad),  //
                      m_distance * qSin(pitchRad) - 8,             //
                      m_distance * qCos(pitchRad) * qSin(yawRad)),
                  QVector3D(0, -8, 0), QVector3D(0, 1, 0));

    // Set modelview-projection matrix
    m_program->setUniformValue("mvp_matrix", m_projection * matrix);

    m_scene->draw(m_program);
    m_program->release();
}

void SkinOpenGLWindow::updateScene(SkinModel* skin)
{
    if (skin && m_scene) {
        m_scene->setMode(skin->getModel() == SkinModel::SLIM);
        m_scene->setSkin(skin->getTexture());
        update();
    }
}
void SkinOpenGLWindow::updateCape(const QImage& cape)
{
    if (m_scene) {
        m_scene->setCapeVisible(!cape.isNull());
        m_scene->setCape(cape);
        update();
    }
}

QColor calculateContrastingColor(const QColor& color)
{
    constexpr float contrast = 0.2;
    auto luma = Rainbow::luma(color);
    if (luma < 0.5) {
        return Rainbow::lighten(color, contrast);
    } else {
        return Rainbow::darken(color, contrast);
    }
}

QImage generateChessboardImage(int width, int height, int tileSize, QColor baseColor)
{
    QImage image(width, height, QImage::Format_RGB888);
    auto white = baseColor;
    auto black = calculateContrastingColor(baseColor);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            bool isWhite = ((x / tileSize) % 2) == ((y / tileSize) % 2);
            image.setPixelColor(x, y, isWhite ? white : black);
        }
    }
    return image;
}

void SkinOpenGLWindow::generateBackgroundTexture(int width, int height, int tileSize)
{
    m_backgroundTexture = new QOpenGLTexture(generateChessboardImage(width, height, tileSize, m_baseColor));
    m_backgroundTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_backgroundTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
}

void SkinOpenGLWindow::renderBackground()
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);  // Disable depth buffer writing
    m_backgroundTexture->bind();
    QMatrix4x4 matrix;
    m_program->setUniformValue("mvp_matrix", matrix);
    m_program->setUniformValue("texture", 0);
    m_background->draw(m_program);
    m_backgroundTexture->release();
    glDepthMask(GL_TRUE);  // Re-enable depth buffer writing
    glEnable(GL_DEPTH_TEST);
}

void SkinOpenGLWindow::wheelEvent(QWheelEvent* event)
{
    // Adjust distance based on scroll
    int delta = event->angleDelta().y();  // Positive for scroll up, negative for scroll down
    m_distance -= delta * 0.01f;          // Adjust sensitivity factor
    m_distance = qMax(16.f, m_distance);  // Clamp distance
    update();                             // Trigger a repaint
}
void SkinOpenGLWindow::setElytraVisible(bool visible)
{
    if (m_scene)
        m_scene->setElytraVisible(visible);
}
