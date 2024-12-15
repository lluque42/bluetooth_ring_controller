class BluetoothService {
  constructor() {
    this.device = null;
    this.characteristic = null;
    this.subscribers = new Set();
  }

  async connect() {
    try {
      this.device = await navigator.bluetooth.requestDevice({
        filters: [
          { namePrefix: 'Filigring' } // Ajusta según el nombre de tu dispositivo
        ],
        optionalServices: ['4fafc201-1fb5-459e-8fcc-c5c9c331914b'] // UUID de tu servicio ESP32
      });

      const server = await this.device.gatt.connect();
      const service = await server.getPrimaryService('4fafc201-1fb5-459e-8fcc-c5c9c331914b');
      this.characteristic = await service.getCharacteristic('beb5483e-36e1-4688-b7f5-ea07361b26a8');

      await this.characteristic.startNotifications();
      this.characteristic.addEventListener('characteristicvaluechanged', this.handleValueChange.bind(this));

      console.log('Bluetooth conectado exitosamente');
    } catch (error) {
      console.error('Error conectando Bluetooth:', error);
      throw error;
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
    }
  }
}

export const bluetoothService = new BluetoothService();
