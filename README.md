# Introducere in Robotica - Tema 1

Această temă simulează o stație de încărcare pentru un vehicul electric, folosind mai multe LED-uri și butoane.

## Componente:

4x LED-uri (pentru a simula procentul de încărcare);
  
1x LED RGB (pentru starea de liber sau ocupat);
  
2x Butoane (pentru start încărcare și stop încărcare);
  
8x Rezistoare (6x 220/330ohm, 2x 1K);
  
Breadboard;
  
Linii de legătură.

## Schema:

![schema electrica](https://github.com/user-attachments/assets/0eccb6e1-28a5-4e45-a900-2a9c1ef40fca)

## Descriere:

Starea stației este ‘liberă’. Loader-ul este stins, iar led-ul pentru disponibilitate este verde. Se apasă butonul pentru start. Led-ul pentru disponibilitate se face roșu, iar încărcarea începe prin aprinderea primului LED L1. Led-ul 1 clipește timp de 3s, celelalte fiind stinse. Dupa încărcarea primului procent de 25% led-ul rămâne aprins și se trece la următorul led, care va începe să clipească. La finalizarea încărcării toate led-urile vor clipi simultan de 3 ori, iar apoi se vor stinge, pentru a semnaliza finalizarea procesului. Led-ul pentru disponibilitate se face verde. Dacă oricând de la pornirea încărcării până la finalizarea acesteia este apăsat lung (min 1s) butonul de stop, încărcarea se întrerupe prin animația de final (toate led-urile clipesc de 3 ori), iar led-ul pentru disponibilitate devine verde.

## Setup:

![IMG_20241020_174643](https://github.com/user-attachments/assets/7fd517db-ce67-4c57-8983-0457880e05ee)

## Video:

[<img src="https://img.youtube.com/vi/VCjNsLooLII/maxresdefault.jpg">](https://youtu.be/VCjNsLooLII)

