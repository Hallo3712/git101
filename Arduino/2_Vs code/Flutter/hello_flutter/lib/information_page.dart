import 'package:flutter/material.dart';
import 'package:easy_localization/easy_localization.dart';

class InformationPage extends StatelessWidget {
  const InformationPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('information_title'.tr()),
      ), // แปลข้อความใน json
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text(
              'app_version'.tr(args: ['1.5']),
              style: const TextStyle(fontSize: 20),
            ),
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: () {
                // สลับภาษา
                if (context.locale.languageCode == 'en') {
                  context.setLocale(const Locale('th'));
                } else {
                  context.setLocale(const Locale('en'));
                }
              },
              child: Text('switch_language'.tr()),
            ),
          ],
        ),
      ),
    );
  }
}
