// import { Outlet } from "react-router-dom";
// import Navbar from "./components/Navbar";

// const App = () => {
//   return (
//     <div className="w-full p-6">
//       <Navbar />
//       <Outlet />
//     </div>
//   );
// };
// export default App;

import React, { useState } from 'react';
import './App.css';

function App() {
  const [selectedImage, setSelectedImage] = useState(null);

  const handleImageUpload = (event) => {
    setSelectedImage(URL.createObjectURL(event.target.files[0]));
  };

  const handleImageProcess = () => {
    // Aquí es donde agregarías la lógica para procesar la imagen
    alert("La imagen ha sido enviada para su análisis.");
  };

  return (
    <div className="app">
      <header className="app-header">
        <h1>ServiceDeskai</h1>
        <p>Sube una imagen para poder procesarla <br></br>Geolocalizar y reportar</p>
      </header>

      <div className="upload-section">
        <div className="image-upload">
          <label className="custom-file-upload">
            <input className="file-input" type="file" accept="image/*" onChange={handleImageUpload} />
            Choose File
          </label>
          {selectedImage && (
            <div className="image-preview">
              <img src={selectedImage} alt="Preview" />
            </div>
          )}
        </div>
        <button onClick={handleImageProcess} className="process-button">
          Procesar Imagen
        </button>
      </div>

      <footer className="app-footer">
        <p>Alcarden 2024</p>
      </footer>
    </div>
  );
}

export default App;
