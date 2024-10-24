import React, { useState } from 'react';
import './App.css';
import CameraComponent from './components/Camera';

function App() {
  const [selectedImage, setSelectedImage] = useState(null);
  const [showCamera, setShowCamera] = useState(false);

  // Manejar la carga de imágenes desde el input
  const handleImageUpload = (event) => {
    setSelectedImage(URL.createObjectURL(event.target.files[0]));
  };

  // Procesar la imagen (puedes agregar la lógica aquí)
  const handleImageProcess = () => {
    alert("La imagen ha sido enviada para su análisis.");
  };

  // Mostrar u ocultar la cámara
  const toggleCamera = () => {
    setShowCamera((prev) => !prev);
  };

  return (
    <div className="app">
      <header className="app-header">
        <h1>ServiceDeskai</h1>
        <p>Sube una imagen para poder procesarla <br></br>Geolocalizar y reportar</p>
      </header>

      <div className="upload-section">
        {/* Sección para subir la imagen desde el input */}
        <div className="image-upload">
          <label className="custom-file-upload">
            <input className="file-input" type="file" accept="image/*" onChange={handleImageUpload} />
            Subir Imagen
          </label>
          {selectedImage && (
            <div className="image-preview">
              <img src={selectedImage} alt="Preview" />
            </div>
          )}
        </div>

        {/* Botón para procesar la imagen */}
        <button onClick={handleImageProcess} className="process-button">
          Procesar Imagen
        </button>

        {/* Botón para activar la cámara */}
        <button onClick={toggleCamera} className="camera-button btn">
          {showCamera ? "Cerrar Cámara" : "Abrir Cámara"}
        </button>

        {/* Mostrar el componente de la cámara cuando el estado 'showCamera' sea verdadero */}
        {showCamera && <CameraComponent />}
      </div>

      <footer className="app-footer">
        <p>Alcarden 2024</p>
      </footer>
    </div>
  );
}

export default App;
