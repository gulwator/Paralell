# Karakterek számlálása text fájlokban OpenCL-lel

Ez a repository a Miskolci Egyetem Párhuzamos Eszközök Programozása tantárgy beadandó feladatát tartalmazza.

## Projekt áttekintés

A projekt célja párhuzamos és szekvenciális karakter számlálási módszerek megvalósítása text fájlokban OpenCL használatával. A feladat célja bemutatni, hogy a GPU-n végzett párhuzamos számítások jelentősen gyorsabbak lehetnek a CPU alapú szekvenciális megoldásoknál. A mérésekhez 250 000-től 6 250 000-ig 250 000 karakterenként haladtam felfelé.

## Főbb jellemzők

* **Párhuzamos karakter számlálás:** Hatékonyan kihasználja a GPU-k párhuzamos feldolgozási képességeit, jelentősen csökkentve a feldolgozási időt.
* **Szekvenciális karakter számlálás:** Referenciaérték a teljesítmény-összehasonlításhoz.
* **Eszközök teljesítményének elemzése:** Érdekes megfigyelések különböző eszközök teljesítményéről. Például egy 2025-ös közép-alsó kategóriás eszköz gyakran nagyobb gyorsítást mutat, mint a legújabb csúcskategóriás modellek.

## Eredmények

A mérések eredménye a következő linken érhetőek el: https://docs.google.com/spreadsheets/d/1BqnUcOeJpeyD3kdywU9YcK61wsp4fsFxDHLmfUWWL-8/edit?usp=sharing

A mérések alapján a párhuzamos feldolgozás jelentősen csökkentheti a futási időt a CPU-alapú megoldásokhoz képest. Azonban a gyorsulás mértéke nagyban függ a használt hardver specifikációitól.

## Követelmények

* OpenCL 2.2 vagy újabb
* Kompatibilis GPU vagy CPU eszköz
* C/C++ fordító

## Használat

A projekt fordításához és futtatásához használd a mellékelt makefile-t vagy az alábbi parancsokat:

```bash
	gcc main.c src/txt_loader.c src/kernel_loader.c -o main.exe -Iinclude -lOpenCL
        gcc -o szekvencial szekvencialis.c
```

## Licenc

Ez a projekt MIT licenc alatt érhető el.

---
