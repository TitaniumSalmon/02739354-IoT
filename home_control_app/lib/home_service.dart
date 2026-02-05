import 'dart:convert';
import 'package:http/http.dart' as http;

class HomeService {
  static const String controlUrl =
      'https://api.netpie.io/v2/device/message?topic=home/device_control';
  static const String statusUrl = 'https://api.netpie.io/v2/device/shadow/data';

  // สำหรับการควบคุมหลอดไฟ (setLedStatus)
  static const String controlClientId = '98344357-f5e5-4d62-8257-f6404db9996d'; // of Mobile App
  static const String controlToken = 'R3hyR3fUBk26TqVeYE2xoudPC36nz3vX'; // of Mobile App

  // สำหรับการอ่านสถานะหลอดไฟ (getLedStatus)
  static const String statusClientId = 'd4eb1908-baf9-462f-9548-65c677d84b1e';  // of ESP32
  static const String statusToken = 'w7uMyBTGKrUA5hUdTRiehvNGk8Ykk6j9';  // of ESP32

  Map<String, String> get _controlHeaders => {
    'Content-Type': 'application/json',
    'Authorization': 'Device $controlClientId:$controlToken',
  };

  Map<String, String> get _statusHeaders => {
    'Content-Type': 'application/json',
    'Authorization': 'Device $statusClientId:$statusToken',
  };

  Future<bool> getLedStatus() async {
    final response = await http.get(
      Uri.parse(statusUrl),
      headers: _statusHeaders,
    );
    if (response.statusCode == 200) {
      final data = json.decode(response.body);
      return data['data']['led1'] == 1;
    } else {
      throw Exception('Failed to fetch LED status');
    }
  }

  Future<void> setLedStatus(bool isOn) async {
    final payload = json.encode({
      'data': {'led1': isOn ? 1 : 0},
    });
    final response = await http.put(
      Uri.parse(controlUrl),
      headers: _controlHeaders,
      body: payload,
    );
    if (response.statusCode != 200) {
      throw Exception('Failed to send LED command');
    }
  }
}
