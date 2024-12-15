import React, { useState } from 'react';
import './App.css';
import CameraComponent from './components/Camera';
// Cambiar la importación del ícono
import icon from '/logo_o.png';  // Importar directamente desde la raíz

function App() {
  const [selectedImage, setSelectedImage] = useState(null);
  const [showCamera, setShowCamera] = useState(false);
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [username, setUsername] = useState('Usuario');
  const [loginUsername, setLoginUsername] = useState('');
  const [loginPassword, setLoginPassword] = useState('');

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
    setUsername(loginUsername); // Establecer el nombre de usuario ingresado
  };

  const handleLogout = () => {
    setIsLoggedIn(false);
    setSelectedImage(null);
    setShowCamera(false);
    setLoginUsername(''); // Limpiar el campo de nombre de usuario al desloguear
    setLoginPassword(''); // Limpiar el campo de contraseña al desloguear
  };

  return (
    <div className="app">
      <header className={`app-header ${isLoggedIn ? 'logged-in' : 'logged-out'}`}>
        <div className="header-content">
          <img src={icon} alt="Icono" className="app-icon" />
          <h1 className="app-title">{isLoggedIn ? "Ornamentech" : "Ornamentech"}</h1>
        </div>
        {isLoggedIn ? (
        <div className="user">
          <span >{username}</span>
        </div>
        ) : null}
        {isLoggedIn ? (
         <div className="user-info">
           <button onClick={handleLogout} className="logout-button">Log-out</button>
         </div>
        ) : null}
        
      </header>

      {!isLoggedIn ? (
        <div className="login-section">
          <input 
            type="text" 
            placeholder="Nombre de usuario" 
            value={loginUsername}
            onChange={(e) => setLoginUsername(e.target.value)} 
            className="login-input"
          />
          <input 
            type="password" 
            placeholder="Contraseña" 
            value={loginPassword}
            onChange={(e) => setLoginPassword(e.target.value)} 
            className="login-input"
          />
          <button onClick={handleLogin} className="login-button btn">Iniciar Sesión</button>
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
        <p>Ornamentech 2024</p>
      </footer>
    </div>
  );
}

export default App;
