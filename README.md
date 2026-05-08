# Cluster Computing – Sommersemester 2026

<div align="center">

![Status](https://img.shields.io/badge/Status-In%20Progress-blue)
![Module](https://img.shields.io/badge/Module-Cluster%20Computing-orange)

</div>

## Beschreibung

Dieses Repository enthält die Übungsaufgaben des Moduls **"Cluster Computing"** bei **Barry Linnert** an der **Freien Universität Berlin**.

Die aktuelle Abgabe befindet sich im `main`-Branch und wird bei jeder Abgabe erweitert. 

`/Theorie` -> Theorieaufgaben im Markdown-Format

`/Task's` -> Praxisaufgaben-Implementierungen

Ausführungsdetails und Abgabestatus zu den einzelen Abgaben befinden sich unten ⬇️

## Ausführungsdetails

### Aufgabe 01

#### Idee

Wir befolgen eine einfache Idee. Der Prozess 0 beschäftigt sich mit der Erstellung, dem Versenden und dem Aufsammeln des Ergebnisses. 
Der Prozess 0 versendet ein Teil der Zeilen der Matrix A an alle beteiligten Prozesse, den er anhand der Anzahl und des notwendigen Versatzes berechnet hat.  
Die Matrix B wird mit Hilfe des Broadcastes an alle Prozesse weiter geleitet.
Die Prozesse berechnen nur die Muktiplikation ihrer Zeilen der Matrix A mit allen Spalten der Matrix B.
Diese Ergebnisse werden im Prozess 0, wieder aufgesammelt und in der Ergebnismatrix gespeichert.

Für das Benchmarking werden auf Grundlage der gegebenen Speicherkapazität die Matrixparameter (gespeichert in Workload) so gewählt, dass die maximale Auslastung durch Verwendung möglichst quadraticher Matrizen A und B erreicht wird. 

#### Benchmarking

In Benchmarking logs sieht man bei voller CPU Auslastung die zeitliche Verbesserung, die ungefähr umgekehrt proportional der Anzahl genutzter Prozessoren ist. 

Mit einem Prozessor braucht das Program ca. 50 sec, mit 2 ca. 25 und mit 4 ca. 12 sec.

Dieser Effekt der umgekehrten Proportionalität ist erwartet, da wir nur die Matrix A in Subprobleme 
ungefähr gleicher Größe auftrennen und jeder Prozess dann mit eigenem Subproblem der Größe ~ 1/n 
beschäftigt ist. 

## Contributors

| Name | Matrikelnummer |
|------|----------------|
| Bennet Worrmann | 5425047 |
| Xenia Krotov | 4858995 |
