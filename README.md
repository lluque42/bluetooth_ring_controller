# mern-stack-example
Mern Stack code for the [Mern Tutorial](https://www.mongodb.com/languages/mern-stack-tutorial)

# En el enlace de Vercel service-deskai-42-malaga.vercel.app hay un error en el Deploy por lo que la conexión con la API que devuelve una descripción de la foto solo funciona en local.

[![CI](https://github.com/mongodb-developer/mern-stack-example/actions/workflows/main.yaml/badge.svg)](https://github.com/mongodb-developer/mern-stack-example/actions/workflows/main.yaml)

## How To Run
Create the file `mern/server/config.env` with your Atlas URI and the server port:
```
ATLAS_URI=mongodb+srv://<username>:<password>@sandbox.jadwj.mongodb.net/
PORT=5050
```

Start server:
```
cd mern/server
npm install
npm start
```

Start Web server
```
cd mern/client
npm install
npm run dev
```

## Disclaimer

Use at your own risk; not a supported MongoDB product
