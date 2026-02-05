import 'package:flutter/material.dart';
import 'home_service.dart';
import 'mqtt_service.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  final HomeService _service = HomeService();
  late MqttService _mqtt;
  final String mobileClientId = '98344357-f5e5-4d62-8257-f6404db9996d'; // of Mobile App
  final String mobileToken = 'R3hyR3fUBk26TqVeYE2xoudPC36nz3vX'; // of Mobile App
  final String mobileSecret = '3eURfEWLdiJkXZMkXqFSmeYfNCRZy6uk'; // of Mobile App

  bool _isLedOn = false;

  @override
  void initState() {
    super.initState();
    _mqtt = MqttService(
      clientId: mobileClientId,
      token: mobileToken,
      secret: mobileSecret,
      onLedStatusChanged: (status) {
        setState(() => _isLedOn = status);
      },
    );
    _mqtt.connect();
  }

  @override
  void dispose() {
    _mqtt.disconnect();
    super.dispose();
  }

  Future<void> _toggleLed(bool value) async {
    setState(() => _isLedOn = value);
    await _service.setLedStatus(value);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Home Device Controller'),
        backgroundColor: Colors.blue,
      ),
      body: SizedBox(
        width: double.infinity,
        height: double.infinity,
        child: Card(
          margin: const EdgeInsets.all(10),
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(15),
          ),
          color: Colors.white,
          elevation: 1,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Text(
                _isLedOn ? 'LED is ON' : 'LED is OFF',
                style: const TextStyle(
                  fontSize: 24,
                  fontWeight: FontWeight.bold,
                ),
              ),
              Image.asset(
                _isLedOn ? 'assets/led-on.png' : 'assets/led-off.png',
                width: 150,
              ),
              const SizedBox(height: 20),
              Switch(
                value: _isLedOn,
                activeColor: Colors.amber,
                onChanged: _toggleLed,
              ),
            ],
          ),
        ),
      ),
    );
  }
}
