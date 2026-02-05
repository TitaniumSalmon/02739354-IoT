import 'dart:async';
import 'package:flutter/material.dart';
import 'home_service.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  final HomeService _service = HomeService();
  bool _isLedOn = false;
  bool _isLoading = true;
  Timer? _pollingTimer;

  @override
  void initState() {
    super.initState();
    _loadLedStatus();
    _startPolling();
  }

  void _startPolling() {
    _pollingTimer = Timer.periodic(const Duration(seconds: 3), (_) {
      _loadLedStatus();
    });
  }

  @override
  void dispose() {
    _pollingTimer?.cancel();
    super.dispose();
  }

  Future<void> _loadLedStatus() async {
    try {
      final status = await _service.getLedStatus();
      setState(() {
        _isLedOn = status;
        _isLoading = false;
      });
    } catch (e) {
      setState(() => _isLoading = false);
      // handle error
    }
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
          child:
              _isLoading
                  ? const CircularProgressIndicator()
                  : Column(
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
