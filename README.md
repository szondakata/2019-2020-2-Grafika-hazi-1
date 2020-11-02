# 2019-2020-2-Grafika-hazi-1
A BME-VIK VIIIAB07 tárgykódú Számítógépes Grafika 2020 tavaszi félévének első házi feladata. Készült kizárólag okulásul, nem alapanyagnak.

A feladat:
A Szíriusz csillagképből érkező földönkívülieknek megtetszett a Word, Powerpoint, stb. jellegű 2D-s rajzoló program, de azt közvetlenül nem használhatják, ugyanis ők más geometriával dolgoznak. Önt bízták meg a program adaptálásával arra az egyszerű esetre, amikor három pontra egy háromszöget kell illeszteni. Szerencsére adtak egy szótárat, ami a szíriuszi geometriai fogalmakat megfelelteti az általunk használt euklideszi geometriai fogalmaknak. A Szíriusz sík az euklideszi sík egység sugarú köre, amit alapkörnek neveznek. Miközben az euklideszi síkot a komplex számokkal lehet számszerűsíteni, a Szíriusz síkot az egységnél kisebb abszolút értékű komplex számokkal. Amíg az euklideszi sík metrikája |dz|=sqrt(dx^2+dy^2) a Szíriusz sík metrikája |dz|/(1-|z|^2). A Szíriusz egyenes egy olyan euklideszi körív, amely az alapkörre merőlegesen érkezik. A feladat három egér klikk után a három pont által definiált háromszöget a háttértől eltérő színnel kitölteni, az éleket ezektől eltérő színnel felrajzolni, és a szabványos kimenetre a három szögeit és oldalainak hosszát kiírni.

Ismert hibák a megoldásban:
- alacsony felbontás esetén a kitöltő algoritmus "áthúzhat" az íven. A megoldás jó közelítés a fülvágó algoritmus kikerülésére
- hibás a szög- és a hossz-számítás

Értékelés:
- 2 megszerezhető pontból 2, javító komment nélkül. Így a fentebb ismertetett hibákra nem fogok megoldást keresni.

HA HÁZIT ÍRSZ:
 - Szívesen segítek, ha fel tudsz mutatni valamit, valószínűleg el tudom mondani, hogy hol lehet hibás. Ha nem grafika-probléma, henem inkább C++ jellegű a baj, mindenképp olvasd el a leadó portálon felsorolt gyakori hibákat, és ellenőrizd a kódod: azon, hogy nem fut, nem tudok segíteni. Sőt! Nagy valószínűséggel fordítani és futtatni sem fogom tudni a kódod, csak a szövegben tudok hibákat keresni. Nem vagyok nagy elsőházi mester (ahogyan ez a fülvágás kikerüléséből sejthető), ezt előre leszögezem, a raytraceing jobban megy.
 - Ha elindulni nem sikerül: a BSS youtube-ján van fent konzi anyag, illetve Szirmay-Kalos László youtube-ján vannak rövid kódolós videók. Ezek alapján el lehet indulni. Érdemes nem csak a zöld háromszög dummy-ból kiindulni, ott vannak a smoothtriangle, a bezier cpp-k, amelyek megmutatják a bonyolultabb programok szervezésének módját. A glut működését ezeken szerintem jobban meg lehet érteni.

HA NEM BME-SKÉNT ÉRDEKEL A GRAFIKA:
 - Ha az egyetemi anyagokat szeretnéd külsősként megszerezni, akkor kérd az előadótól, oda fogja adni. Hagyj engem ki abból, hogy nem mersz levelet írni olyannak, akinek dr-ral kezdődik a neve.
 - Szívesen segítek nem-adminisztratív kérdésekben.

Ps.
Messengeren ne keress, emailen inkább
