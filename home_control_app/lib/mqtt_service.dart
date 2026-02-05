import 'dart:convert';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

class MqttService {
  final String clientId;
  final String token;
  final String secret;
  final Function(bool) onLedStatusChanged;

  late MqttServerClient _client;

  MqttService({
    required this.clientId,
    required this.token,
    required this.secret,
    required this.onLedStatusChanged,
  });

  Future<void> connect() async {
    _client = MqttServerClient('broker.netpie.io', clientId);
    _client.port = 1883;
    _client.keepAlivePeriod = 20;
    _client.onDisconnected = _onDisconnected;
    _client.onConnected = _onConnected;

    _client.connectionMessage =
        MqttConnectMessage()
            .withClientIdentifier(clientId)
            .authenticateAs(token, secret)
            .startClean();

    try {
      await _client.connect();
    } catch (e) {
      _client.disconnect();
    }

    if (_client.connectionStatus!.state == MqttConnectionState.connected) {
      _client.subscribe('@msg/home/device_status', MqttQos.atMostOnce);
      _client.updates!.listen(_onMessage);
    }
  }

  void _onMessage(List<MqttReceivedMessage<MqttMessage>> events) {
    final recMsg = events[0].payload as MqttPublishMessage;
    final payload = MqttPublishPayload.bytesToStringAsString(
      recMsg.payload.message,
    );

    try {
      final decoded = json.decode(payload);
      final ledValue = decoded['data']['led1'];

      if (ledValue is int) {
        final isOn = ledValue == 1;
        onLedStatusChanged(isOn);
      }
    } catch (e) {
      print('Error decoding JSON: $e');
    }
  }

  void _onConnected() => print('MQTT Connected');
  void _onDisconnected() => print('MQTT Disconnected');

  void disconnect() {
    _client.disconnect();
  }
}
