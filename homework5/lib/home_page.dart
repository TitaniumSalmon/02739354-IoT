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
  bool _isRedLightOn = false;
  bool _isYellowLightOn = false;
  bool _isGreenLightOn = false;
  bool _isBuzzerOn = false;
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
        _isRedLightOn = status['led1'] ?? false;
        _isYellowLightOn = status['led2'] ?? false;
        _isGreenLightOn = status['led3'] ?? false;
        _isBuzzerOn = status['buzzer'] ?? false;
        _isLoading = false;
      });
    } catch (e) {
      setState(() => _isLoading = false);
      // handle error
    }
  }

  Future<void> _toggleLed(bool led1, bool led2, bool led3, bool buzzer) async {
    //setState(() => _isRedLightOn = led1);
    await _service.setLedStatus(led1, led2, led3, buzzer);
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
          child: _isLoading
              ? const CircularProgressIndicator()
              : Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Row(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        Column(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            Text(
                              _isRedLightOn ? 'LED is ON' : 'LED is OFF',
                              style: const TextStyle(
                                fontSize: 24,
                                fontWeight: FontWeight.bold,
                              ),
                            ),
                            Image.asset(
                              _isRedLightOn
                                  ? '../assets/images/BG_BuildingCorridor.jpg'
                                  : '../assets/images/BG_BuildingCorridor_Night.jpg',
                              width: 300,
                            ),
                            const SizedBox(height: 20),
                            Switch(
                              value: _isRedLightOn,
                              activeColor: const Color.fromARGB(
                                255,
                                235,
                                69,
                                69,
                              ),
                              onChanged: (value) => _toggleLed(
                                value,
                                _isYellowLightOn,
                                _isGreenLightOn,
                                _isBuzzerOn,
                              ),
                            ),
                          ],
                        ),

                        const SizedBox(width: 50),

                        Column(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            Text(
                              _isYellowLightOn ? 'LED is ON' : 'LED is OFF',
                              style: const TextStyle(
                                fontSize: 24,
                                fontWeight: FontWeight.bold,
                              ),
                            ),
                            Image.asset(
                              _isYellowLightOn
                                  ? '../assets/images/BG_RestingLounge.jpg'
                                  : '../assets/images/BG_RestingLounge_Night.jpg',
                              width: 300,
                            ),
                            const SizedBox(height: 20),
                            Switch(
                              value: _isYellowLightOn,
                              activeColor: const Color.fromARGB(
                                255,
                                235,
                                69,
                                69,
                              ),
                              onChanged: (value) => _toggleLed(
                                _isRedLightOn,
                                value,
                                _isGreenLightOn,
                                _isBuzzerOn,
                              ),
                            ),
                          ],
                        ),
                      ],
                    ),

                    SizedBox(height: 50),

                    Row(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        Column(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            Text(
                              _isGreenLightOn ? 'LED is ON' : 'LED is OFF',
                              style: const TextStyle(
                                fontSize: 24,
                                fontWeight: FontWeight.bold,
                              ),
                            ),
                            Image.asset(
                              _isGreenLightOn
                                  ? '../assets/images/BG_StudentRoom_Night2.jpg'
                                  : '../assets/images/BG_StudentRoom_Night.jpg',
                              width: 300,
                            ),
                            const SizedBox(height: 20),
                            Switch(
                              value: _isGreenLightOn,
                              activeColor: const Color.fromARGB(
                                255,
                                235,
                                69,
                                69,
                              ),
                              onChanged: (value) => _toggleLed(
                                _isRedLightOn,
                                _isYellowLightOn,
                                value,
                                _isBuzzerOn,
                              ),
                            ),
                          ],
                        ),

                        SizedBox(width: 50),

                        Column(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            Text(
                              _isBuzzerOn ? 'Buzzer is ON' : 'Buzzer is OFF',
                              style: const TextStyle(
                                fontSize: 24,
                                fontWeight: FontWeight.bold,
                              ),
                            ),
                            Image.asset(
                              _isBuzzerOn
                                  ? '../assets/images/Speaker_On.jpg'
                                  : '../assets/images/Speaker_Off.jpg',
                              width: 300,
                            ),
                            const SizedBox(height: 20),
                            Switch(
                              value: _isBuzzerOn,
                              activeColor: const Color.fromARGB(
                                255,
                                235,
                                69,
                                69,
                              ),
                              onChanged: (value) => _toggleLed(
                                _isRedLightOn,
                                _isYellowLightOn,
                                _isGreenLightOn,
                                value,
                              ),
                            ),
                          ],
                        ),
                      ],
                    ),
                  ],
                ),
        ),
      ),
    );
  }
}
