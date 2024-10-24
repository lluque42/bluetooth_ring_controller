import express from "express";
import cors from "cors";
import records from "./routes/record.js";

const PORT = process.env.PORT || 5050;
const app = express();

app.use(cors());
app.use(express.json());
app.use("/record", records);

const express = require("express");
const mongoose = require("mongoose");
const fs = require("fs");
const path = require("path");

// Middleware para procesar JSON
app.use(express.json({ limit: '50mb' }));

// Conectar a MongoDB (puedes cambiar la conexión según tu configuración)
mongoose.connect('mongodb://localhost:27017/cameraApp', {
  useNewUrlParser: true,
  useUnifiedTopology: true,
});

// Esquema de Mongoose para almacenar las imágenes
const imageSchema = new mongoose.Schema({
  imageData: Buffer,
  contentType: String,
});

const Image = mongoose.model("Image", imageSchema);

// Ruta para recibir la imagen
app.post("/api/upload", async (req, res) => {
  try {
    const { image } = req.body;

    // Decodificar la imagen base64
    const base64Data = image.replace(/^data:image\/png;base64,/, "");
    const buffer = Buffer.from(base64Data, "base64");

    // Crear el documento en MongoDB
    const newImage = new Image({
      imageData: buffer,
      contentType: "image/png",
    });

    await newImage.save();
    res.json({ message: "Imagen subida correctamente!" });
  } catch (error) {
    console.error("Error al subir la imagen: ", error);
    res.status(500).json({ message: "Error en el servidor" });
  }
});

// start the Express server
app.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});