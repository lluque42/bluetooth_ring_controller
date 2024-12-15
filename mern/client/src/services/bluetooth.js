class BluetoothService {
  constructor() {
    this.device = null;
    this.characteristic = null;
    this.subscribers = new Set();
  }

  isWebBluetoothAvailable() {
    if (!navigator.bluetooth) {
      console.error('Web Bluetooth API no está disponible en este navegador');
      return false;
    }
    return true;
  }

  async connect() {
    if (!this.isWebBluetoothAvailable()) {
      throw new Error('Bluetooth no disponible en este navegador');
    }

    try {
      console.log('Solicitando dispositivo Bluetooth...');
      this.device = await navigator.bluetooth.requestDevice({
        // Acepta cualquier dispositivo que anuncie el servicio que buscamos
        filters: [
          { services: ['4fafc201-1fb5-459e-8fcc-c5c9c331914b'] },
          { namePrefix: 'Filigrind' }
        ],
        optionalServices: ['4fafc201-1fb5-459e-8fcc-c5c9c331914b']
      });

      console.log('Conectando al dispositivo...');
      const server = await this.device.gatt.connect();

      console.log('Obteniendo servicio...');
      const service = await server.getPrimaryService('4fafc201-1fb5-459e-8fcc-c5c9c331914b');

      console.log('Obteniendo característica...');
      this.characteristic = await service.getCharacteristic('beb5483e-36e1-4688-b7f5-ea07361b26a8');

      await this.characteristic.startNotifications();
      this.characteristic.addEventListener('characteristicvaluechanged', this.handleValueChange.bind(this));

      console.log('Bluetooth conectado exitosamente');
      return true;
    } catch (error) {
      console.error('Error detallado en la conexión Bluetooth:', error);
      throw new Error(`Error conectando Bluetooth: ${error.message}`);
    }
  }

  handleValueChange(event) {
    const value = new TextDecoder().decode(event.target.value);
    this.subscribers.forEach(callback => callback(value));
  }

  subscribe(callback) {
    this.subscribers.add(callback);
    return () => this.subscribers.delete(callback);
  }

  disconnect() {
    if (this.device && this.device.gatt.connected) {
      this.device.gatt.disconnect();
      console.log('Dispositivo Bluetooth desconectado');
    }
    this.device = null;
    this.characteristic = null;
  }

  isConnected() {
    return this.device && this.device.gatt.connected;
  }
}

export const bluetoothService = new BluetoothService();
