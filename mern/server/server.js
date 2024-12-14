import express from "express";
import cors from "cors";
import dotenv from 'dotenv';

// Cargar variables de entorno
dotenv.config();

const app = express();

// Middleware
app.use(cors({
  origin: 'http://localhost:5174',
  credentials: true
}));
app.use(express.json({ limit: '50mb' }));

// Configuración de Hugging Face
const API_URL = "https://api-inference.huggingface.co/models/facebook/detr-resnet-50";

async function query(imageData) {
  console.log("Enviando imagen a Hugging Face...");
  try {
    // Asegurarse de que la imagen esté en formato base64 correcto
    const base64Image = imageData.replace(/^data:image\/(png|jpeg|jpg);base64,/, '');
    
    const response = await fetch(API_URL, {
      headers: { 
        Authorization: `Bearer ${process.env.HUGGING_FACE_API_TOKEN}`,
        'Content-Type': 'application/json'
      },
      method: "POST",
      body: JSON.stringify({ 
        inputs: {
          image: base64Image
        }
      })
    });
    
    if (!response.ok) {
      const error = await response.text();
      console.error("Error de Hugging Face:", error);
      throw new Error(`Error en la API: ${response.status}`);
    }
    
    const result = await response.json();
    console.log("Respuesta de Hugging Face:", result);
    return result;
  } catch (error) {
    console.error("Error en query:", error);
    throw error;
  }
}

// Ruta para procesar imágenes
app.post("/api/upload", async (req, res) => {
  console.log("Recibiendo petición de upload");
  try {
    const { files } = req.body;
    
    if (!files) {
      return res.status(422).json({ error: 'No image data provided' });
    }

    const detections = await query(files);
    
    // Verificar que detections es un array
    console.log("Detecciones sin procesar:", detections);
    
    // Asegurarnos de que enviamos el array completo, no solo el primer elemento
    res.json({ 
      success: true, 
      detections: Array.isArray(detections) ? detections : [detections],
      processedImage: files
    });

  } catch (error) {
    console.error("Error processing image:", error);
    res.status(500).json({ 
      error: "Error processing image",
      details: error.message,
      detections: []
    });
  }
});

const PORT = process.env.PORT || 5050;
app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});