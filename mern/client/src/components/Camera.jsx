import React, { useRef, useState } from "react";

const CameraComponent = () => {
  const videoRef = useRef(null);
  const canvasRef = useRef(null);
  const [imageData, setImageData] = useState(null);

  // Función para pedir permisos de la cámara y mostrar el video en pantalla
  const startCamera = async () => {
    try {
      const stream = await navigator.mediaDevices.getUserMedia({
        video: true,
      });
      videoRef.current.srcObject = stream;
      videoRef.current.play();
    } catch (error) {
      console.error("Error al acceder a la cámara: ", error);
    }
  };

  // Función para capturar la imagen desde el video
  const capturePhoto = () => {
    const context = canvasRef.current.getContext("2d");
    canvasRef.current.width = videoRef.current.videoWidth;
    canvasRef.current.height = videoRef.current.videoHeight;
    context.drawImage(videoRef.current, 0, 0, canvasRef.current.width, canvasRef.current.height);
    
    const image = canvasRef.current.toDataURL("image/png"); // Guardar la imagen en base64
    setImageData(image);
  };

  // Función para subir la foto capturada al backend
  const handleSubmit = async () => {
    if (imageData) {
      try {
        const response = await fetch("/api/upload", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify({ image: imageData }),
        });
        const result = await response.json();
        console.log("Foto subida exitosamente: ", result);
      } catch (error) {
        console.error("Error al subir la imagen: ", error);
      }
    }
  };

  return (
    <div>
      <video ref={videoRef} style={{ width: "100%" }}></video>
      <button onClick={startCamera} className="custom-btn btn-1">Activar Cámara</button>
      <button onClick={capturePhoto} className="custom-btn btn-1">Tomar Foto</button>
      <canvas ref={canvasRef} style={{ display: "none" }}></canvas>
      {imageData && <img src={imageData} alt="captured" style={{ width: "100%" }} />}
      <button onClick={handleSubmit} className="custom-btn btn-1" id="upload-btn">Subir Foto</button>
    </div>
  );
};

export default CameraComponent;
