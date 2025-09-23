import 'package:flutter/material.dart';
import 'package:easy_localization/easy_localization.dart';
import 'home_page.dart';
import 'information_page.dart';
import 'history_page.dart'; // import หน้า history เข้ามาด้วย

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await EasyLocalization.ensureInitialized();

  runApp(
    EasyLocalization(
      supportedLocales: const [Locale('en'), Locale('th')],
      path: 'assets/langs',
      fallbackLocale: const Locale('en'),
      child: const MyApp(),
    ),
  );
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      localizationsDelegates: context.localizationDelegates,
      supportedLocales: context.supportedLocales,
      locale: context.locale,
      initialRoute: '/',
      routes: {
        '/': (context) => const HomePage(),
        '/info': (context) => const InformationPage(),
      },
      onGenerateRoute: (settings) {
        if (settings.name == '/history') {
          final history = settings.arguments as List<String>? ?? [];
          return MaterialPageRoute(
            builder: (context) => HistoryPage(history: history),
          );
        }
        return null;
      },
    );
  }
}
