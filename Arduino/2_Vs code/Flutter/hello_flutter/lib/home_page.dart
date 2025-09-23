import 'package:flutter/material.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  int _counter = 0;
  final List<String> _history = [];

  final List<int> _dropdownValues = [1, 2, 3, 5, 10];
  int _selectedValue = 1;
  final TextEditingController _controller = TextEditingController(text: '1');

  int _getCurrentInputValue() {
    final input = int.tryParse(_controller.text);
    if (input != null && input > 0) {
      return input;
    }
    return _selectedValue;
  }

  void _incrementCounter() {
    final value = _getCurrentInputValue();
    setState(() {
      _counter += value;
      _history.add('+$value');
    });
  }

  void _decrementCounter() {
    final value = _getCurrentInputValue();
    setState(() {
      _counter -= value;
      _history.add('-$value');
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Home Page'), // ✅ เอา v1.3 ออกตามคำขอ
        actions: [
          IconButton(
            icon: const Icon(Icons.history),
            onPressed: () {
              Navigator.pushNamed(context, '/history', arguments: _history);
            },
          ),
          IconButton(
            icon: const Icon(Icons.info_outline), // ✅ ปุ่มข้อมูล
            onPressed: () {
              Navigator.pushNamed(context, '/info');
            },
          ),
        ],
      ),
      body: Padding(
        padding: const EdgeInsets.all(24.0),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            const Text('You have pushed the button this many times:'),
            Text(
              '$_counter',
              style: Theme.of(context).textTheme.headlineMedium,
            ),
            const SizedBox(height: 32),
            Row(
              children: [
                DropdownButton<int>(
                  value: _selectedValue,
                  items: _dropdownValues
                      .map(
                        (val) =>
                            DropdownMenuItem(value: val, child: Text('$val')),
                      )
                      .toList(),
                  onChanged: (val) {
                    if (val != null) {
                      setState(() {
                        _selectedValue = val;
                        _controller.text = val.toString();
                      });
                    }
                  },
                ),
                const SizedBox(width: 8),
                Expanded(
                  child: TextField(
                    controller: _controller,
                    keyboardType: TextInputType.number,
                    decoration: const InputDecoration(
                      border: OutlineInputBorder(),
                      labelText: 'Enter amount',
                    ),
                  ),
                ),
              ],
            ),
          ],
        ),
      ),
      floatingActionButton: Padding(
        padding: const EdgeInsets.only(left: 30),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.end,
          children: [
            FloatingActionButton(
              heroTag: 'btn-decrement', // เพิ่ม tag ไม่ซ้ำ
              onPressed: _decrementCounter,
              tooltip: 'Decrement',
              child: const Icon(Icons.remove),
            ),
            const SizedBox(width: 16),
            FloatingActionButton(
              heroTag: 'btn-increment', // เพิ่ม tag ไม่ซ้ำ
              onPressed: _incrementCounter,
              tooltip: 'Increment',
              child: const Icon(Icons.add),
            ),
          ],
        ),
      ),
    );
  }
}
