import React, { useRef, useState, useEffect } from "react";
import { bluetoothService } from '../services/bluetooth';

const CameraComponent = () => {
  const videoRef = useRef(null);
  const canvasRef = useRef(null);
  const [imageData, setImageData] = useState(null);
  const [isCapturing, setIsCapturing] = useState(false);
  const [intervalId, setIntervalId] = useState(null);
  const [detections, setDetections] = useState([]);
  const [processedImage, setProcessedImage] = useState(null);
  const imageRef = useRef(null);
  const [scale, setScale] = useState({ x: 1, y: 1 });
  const containerRef = useRef(null);
  const [highlightedDetection, setHighlightedDetection] = useState(null);
  const [showCursor, setShowCursor] = useState(false);
  const [cursorPosition, setCursorPosition] = useState({ x: 0, y: 0 });
  const [isBluetoothConnected, setIsBluetoothConnected] = useState(false);
  const SCORE_THRESHOLD = 0.7; // Añadir constante para el umbral

  const startCamera = async () => {
    try {
      const stream = await navigator.mediaDevices.getUserMedia({
        video: true,
      });
      videoRef.current.srcObject = stream;
      videoRef.current.play();
    } catch (error) {
      console.error("Error accessing the camera: ", error);
    }
  };

  const capturePhoto = () => {
    const context = canvasRef.current.getContext("2d");
    canvasRef.current.width = videoRef.current.videoWidth;
    canvasRef.current.height = videoRef.current.videoHeight;
    context.drawImage(videoRef.current, 0, 0, canvasRef.current.width, canvasRef.current.height);
    const image = canvasRef.current.toDataURL("image/jpeg", 1.0); // Especificamos JPEG y calidad máxima
    setImageData(image);
    savePhoto(image);
  };

  const savePhoto = async (image) => {
    try {
      const base64Data = image.split(',')[1];
      console.log("Enviando imagen al servidor...");
      
      const response = await fetch("/api/upload", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ 
          files: base64Data
        }),
      });

      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }

      const result = await response.json();
      console.log("Resultado completo:", result);

      if (result.success && result.detections) {
        // Filtrar detecciones por score
        const filteredDetections = Array.isArray(result.detections) 
          ? result.detections.filter(detection => detection.score >= SCORE_THRESHOLD)
          : [];
        
        console.log("Detecciones filtradas (>70%):", filteredDetections);
        setDetections(filteredDetections);
        setProcessedImage(result.processedImage);
      }
    } catch (error) {
      console.error("Error al subir la imagen:", error);
      setDetections([]);
    }
  };

  const startCapturing = () => {
    setIsCapturing(true);
    capturePhoto();
  };

  const stopCapturing = () => {
    setIsCapturing(false);
    clearInterval(intervalId);
    setIntervalId(null);
  };

  const handleCaptureTrigger = () => {
    if (!isCapturing) {
      startCapturing();
    } else {
      stopCapturing();
    }
  };

  const connectBluetooth = async () => {
    try {
      await bluetoothService.connect();
      setIsBluetoothConnected(true);
    } catch (error) {
      console.error('Error al conectar Bluetooth:', error);
    }
  };

  const calculateScale = (image) => {
    if (containerRef.current) {
      const containerWidth = containerRef.current.clientWidth;
      const imageNaturalWidth = image.naturalWidth;
      return containerWidth / imageNaturalWidth;
    }
    return 1;
  };

  const updateDetectionBoxes = () => {
    if (imageRef.current) {
      const newScale = calculateScale(imageRef.current);
      setScale({ x: newScale, y: newScale });
      
      // Añadir este log para depurar dimensiones
      console.log('Image dimensions:', {
        natural: {
          width: imageRef.current?.naturalWidth,
          height: imageRef.current?.naturalHeight
        },
        container: {
          width: containerRef.current?.clientWidth
        },
        scale: newScale
      });
    }
  };

  const handleDetectionHover = (detection) => {
    setHighlightedDetection(detection);
    if (detection) {
      const centerX = (detection.box.xmin + detection.box.xmax) / 2 * scale.x;
      const centerY = (detection.box.ymin + detection.box.ymax) / 2 * scale.y;
      setCursorPosition({ x: centerX, y: centerY });
      setShowCursor(true);
    } else {
      setShowCursor(false);
    }
  };

  const handleDetectionClick = (detection) => {
    console.log("Detección clickeada:", detection);
    setHighlightedDetection(detection);
  };

  useEffect(() => {
    const handleResize = () => updateDetectionBoxes();
    window.addEventListener('resize', handleResize);
    return () => window.removeEventListener('resize', handleResize);
  }, []);

  useEffect(() => {
    if (imageRef.current && imageRef.current.complete) {
      updateDetectionBoxes();
    }
  }, [imageData, detections]);

  useEffect(() => {
    return () => {
      if (intervalId) {
        clearInterval(intervalId);
      }
    };
  }, [intervalId]);

  useEffect(() => {
    if (isBluetoothConnected) {
      const unsubscribe = bluetoothService.subscribe((value) => {
        if (value === '1') { // O el valor que envíe tu ESP32
          handleCaptureTrigger();
        }
      });

      return () => {
        unsubscribe();
        bluetoothService.disconnect();
      };
    }
  }, [isBluetoothConnected]);

  return (
    <div>
      <video ref={videoRef} style={{ width: "100%" }}></video>
      <button onClick={startCamera} className="custom-btn btn-1">Activar Cámara</button>
      <button 
        onClick={handleCaptureTrigger} 
        className="custom-btn btn-1"
      >
        {isCapturing ? "Detener Captura" : "Iniciar Captura"}
      </button>
      <button 
        onClick={connectBluetooth}
        className={`custom-btn btn-1 ${isBluetoothConnected ? 'connected' : ''}`}
      >
        {isBluetoothConnected ? "BT Conectado" : "Conectar BT"}
      </button>
      <canvas ref={canvasRef} style={{ display: "none" }}></canvas>
      
      <div className="images-container">
        {imageData && (
          <div className="image-box" ref={containerRef}>
            <h3>Imagen con Detecciones:</h3>
            <div style={{ position: 'relative', width: '100%' }}>
              <img 
                ref={imageRef}
                src={imageData} 
                alt="captured" 
                style={{ width: '100%', display: 'block' }}
                onLoad={updateDetectionBoxes}
              />
              {detections.length > 0 && (
                <>
                  <div className="detections-overlay">
                    {detections.map((detection, index) => (
                      <div
                        key={index}
                        className={`detection-box ${highlightedDetection === detection ? 'highlight' : ''}`}
                        style={{
                          position: 'absolute',
                          left: `${detection.box.xmin * scale.x}px`,
                          top: `${detection.box.ymin * scale.y}px`,
                          width: `${(detection.box.xmax - detection.box.xmin) * scale.x}px`,
                          height: `${(detection.box.ymax - detection.box.ymin) * scale.y}px`
                        }}
                        onClick={() => handleDetectionClick(detection)}
                      >
                        <span className="detection-label">
                          {detection.label} ({(detection.score * 100).toFixed(1)}%)
                        </span>
                      </div>
                    ))}
                  </div>

                  <div className="detection-controls">
                    {detections.map((detection, index) => (
                      <button
                        key={index}
                        className="detection-button"
                        onMouseEnter={() => handleDetectionHover(detection)}
                        onMouseLeave={() => handleDetectionHover(null)}
                      >
                        {detection.label}
                        <div 
                          className="confidence-bar"
                          style={{ width: `${detection.score * 100}%` }}
                        />
                      </button>
                    ))}
                  </div>
                </>
              )}
            </div>
          </div>
        )}
      </div>

      {Array.isArray(detections) && detections.length > 0 && (
        <div className="detections-box">
          <h3>Lista de Detecciones Confiables:</h3>
          <ul>
            {detections.map((detection, index) => (
              <li key={index}>
                <strong>{detection.label}</strong>
                <br />
                Confianza: {(detection.score * 100).toFixed(1)}%
                <br />
                Posición: ({detection.box.xmin}, {detection.box.ymin}) - 
                ({detection.box.xmax}, {detection.box.ymax})
              </li>
            ))}
          </ul>
        </div>
      )}
      {Array.isArray(detections) && detections.length === 0 && (
        <div className="detections-box">
          <p>No se encontraron objetos con confianza superior al 70%</p>
        </div>
      )}
    </div>
  );
};

export default CameraComponent;
