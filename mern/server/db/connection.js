import { MongoClient, ServerApiVersion } from "mongodb";
import dotenv from 'dotenv';

// Cargar las variables de entorno desde el archivo config.env
dotenv.config({ path: 'config.env' });

// Usar la URI de ATLAS o Local en función de la disponibilidad
const URI = process.env.ATLAS_URI || process.env.MONGO_URI;

if (!URI) {
  console.error("Error: No se ha encontrado ninguna URI válida para MongoDB.");
  process.exit(1);
}

const client = new MongoClient(URI, {
  serverApi: {
    version: ServerApiVersion.v1,
    strict: true,
    deprecationErrors: true,
  },
});

try {
  // Conectar el cliente al servidor
  await client.connect();
  // Hacer ping para confirmar la conexión exitosa
  await client.db("admin").command({ ping: 1 });
  console.log("Conexión exitosa a MongoDB!");
} catch (err) {
  console.error("Error conectando a MongoDB:", err);
  process.exit(1);  // Salir si falla la conexión
}

let db = client.db("employees");

export default db;
