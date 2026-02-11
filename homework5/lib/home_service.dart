import 'dart:convert';
import 'package:http/http.dart' as http;

class HomeService {
  static const String controlUrl =
      'https://api.netpie.io/v2/device/message?topic=home/device_control';
  static const String statusUrl = 'https://api.netpie.io/v2/device/shadow/data';

  // สำหรับการควบคุมหลอดไฟ (setLedStatus)
  static const String controlClientId = '077cf360-9181-4e4b-b09e-849db1cce9eb'; // of Mobile App
  static const String controlToken = 'NWePseW3recdJsuQ4gUz4qMYRtr2T1Az'; // of Mobile App

  // สำหรับการอ่านสถานะหลอดไฟ (getLedStatus)
  static const String statusClientId = '8af8b2f4-56a9-49a0-965c-00e9ec178220';  // of ESP32
  static const String statusToken = 'Q6J6VXKhpUeK9xjBXGqbLCEuwJiGYcjF';  // of ESP32

  Map<String, String> get _controlHeaders => {
    'Content-Type': 'application/json',
    'Authorization': 'Device $controlClientId:$controlToken',
  };

  Map<String, String> get _statusHeaders => {
    'Content-Type': 'application/json',
    'Authorization': 'Device $statusClientId:$statusToken',
  };

  Future<Map<String, bool>> getLedStatus() async {
    final response = await http.get(
      Uri.parse(statusUrl),
      headers: _statusHeaders,
    );
    if (response.statusCode == 200) {
      final data = json.decode(response.body);
      return {
        'led1': data['data']['led1'] == 1 || data['data']['led1'] == true,
        'led2': data['data']['led2'] == 1 || data['data']['led2'] == true,
        'led3': data['data']['led3'] == 1 || data['data']['led3'] == true,
        'buzzer': data['data']['buzzer'] == 1 || data['data']['buzzer'] == true,
      };
    } else {
      throw Exception('Failed to fetch LED status');
    }
  }

  Future<void> setLedStatus(bool led1, bool led2, bool led3, bool buzzer) async {
    final payload = json.encode({
      'data': {
        'led1': led1 ? 1 : 0,
        'led2': led2 ? 1 : 0,
        'led3': led3 ? 1 : 0,
        'buzzer': buzzer ? 1 : 0,
        },
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
