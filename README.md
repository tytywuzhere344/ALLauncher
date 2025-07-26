<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>ALLauncher README</title>
  <style>
    body {
      font-family: "Segoe UI", sans-serif;
      line-height: 1.6;
      background-color: #1e1e1e;
      color: #f0f0f0;
      padding: 2rem;
      max-width: 800px;
      margin: auto;
    }
    h1, h2, h3 {
      color: #f54291;
    }
    a {
      color: #7ea8ff;
      text-decoration: none;
    }
    a:hover {
      text-decoration: underline;
    }
    code {
      background: #333;
      padding: 0.2rem 0.4rem;
      border-radius: 4px;
      color: #ffcc66;
    }
    pre {
      background: #2a2a2a;
      padding: 1rem;
      overflow-x: auto;
      border-radius: 6px;
    }
    .logo {
      display: block;
      max-width: 150px;
      margin: 0 auto 20px;
      border-radius: 12px;
    }
    .center {
      text-align: center;
    }
  </style>
</head>
<body>
  <img src="logo.png" alt="ALLauncher Logo" class="logo">
  <h1 class="center">ALLauncher</h1>
  <p class="center"><strong>A lightweight Minecraft launcher forked from Prism Launcher. ALLauncher is designed for offline use, flexibility, and a streamlined Linux-first experience.</strong></p>

  <h2>🔧 Features</h2>
  <ul>
    <li>Supports offline and online (Xbox) logins</li>
    <li>Easy modpack and resource management</li>
    <li>Simple, focused UI experience</li>
    <li>Runs on ChromeOS/Linux desktops (.deb available)</li>
    <li>Open source and customizable</li>
  </ul>

  <h2>📦 Download</h2>
  <p>
    Visit the <a href="https://github.com/tytywuzhere344/ALLauncher" target="_blank">
    ALLauncher GitHub Repository</a> to download the latest <code>.deb</code> package and source code.
  </p>

  <h2>🚀 Installation</h2>
  <pre><code>sudo dpkg -i ALLauncher.deb
sudo apt --fix-broken install</code></pre>

  <h2>💡 Building from Source</h2>
  <p>Clone the repo and run:</p>
  <pre><code>git clone --recursive https://github.com/tytywuzhere344/ALLauncher
cd ALLauncher
mkdir build && cd build
cmake ..
make -j$(nproc)</code></pre>

  <h2>🐛 Bug Reports</h2>
  <p>Report issues on the <a href="https://github.com/tytywuzhere344/ALLauncher/issues">GitHub Issues page</a> or email <code>ALLauncher.bugs@gmail.com</code>.</p>

  <h2>📸 Screenshots</h2>
  <p>Visit <a href="https://allauncher.neocities.org" target="_blank">allauncher.neocities.org</a> to preview the interface.</p>

  <h2>📜 License</h2>
  <p>This project is based on Prism Launcher and licensed under the GNU General Public License v3.0.</p>

  <hr />
  <p class="center"><small>&copy; 2025 ALLauncher. Not affiliated with Mojang or Microsoft.</small></p>
</body>
</html>

