import React, { useState } from 'react';
import './App.css';
import CameraComponent from './components/Camera';
import icon from '../public/vite.svg';

function App() {
  const [selectedImage, setSelectedImage] = useState(null);
  const [showCamera, setShowCamera] = useState(false);
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [username, setUsername] = useState('Usuario'); // Nombre de usuario

  const handleImageUpload = (event) => {
    setSelectedImage(URL.createObjectURL(event.target.files[0]));
  };

  const handleImageProcess = () => {
    alert("La imagen ha sido enviada para su análisis.");
  };

  const toggleCamera = () => {
    setShowCamera((prev) => !prev);
  };

  const handleLogin = () => {
    setIsLoggedIn(true);
    setUsername('Peter'); // Cambia esto al nombre que desees
  };

  const handleLogout = () => {
    setIsLoggedIn(false);
    setSelectedImage(null);
    setShowCamera(false);
  };

  return (
    <div className="app">
      <header className={`app-header ${isLoggedIn ? 'logged-in' : 'logged-out'}`}>
        <div className="header-content">
          <img src={icon} alt="Icono" className="app-icon" />
          <h1 className="app-title">{isLoggedIn ? "ServiceDeskai" : "ServiceDeskai"}</h1>
        </div>
        {isLoggedIn && (
          <div className="user-info">
            <span className="user">{username}</span>
            <button onClick={handleLogout} className="logout-button">Desloguear</button>
          </div>
        )}
      </header>

      {!isLoggedIn ? (
        <div className="login-section">
          <button onClick={handleLogin}>Iniciar Sesión</button>
        </div>
      ) : (
        <div className="upload-section">
          <div className="image-upload">
            {selectedImage && (
              <div className="image-preview">
                <img src={selectedImage} alt="Preview" />
              </div>
            )}
          </div>
          <button onClick={toggleCamera} className="camera-button btn">
            {showCamera ? "Cerrar Cámara" : "Abrir Cámara"}
          </button>

          {showCamera && <CameraComponent />}
        </div>
      )}

      <footer className="app-footer">
        <p>Alcarden 2024</p>
      </footer>
    </div>
  );
}

export default App;
