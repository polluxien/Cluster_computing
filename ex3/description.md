### Beschreibung des Algorithmus

Unser paralleles Program funktioniert in  gleicher Weise, wie auch zuvor. Thread 0 kümmert sich um das Aufsammeln von den Ergebnissen und dem gleichzeitigem Verteilen der Daten an die weitere Threads. 
Der Quad-Tree wird aus dem vollständigem Datensatz innerhalb der einzelnen Threads erstellt. Der Algorithmus von * erfordert einen geometrischen Index. Der Quod Tree unterteilt einen vorgegebenen
Bereich rekursiv in vier gleich große Quadranten. Alle Blätter enthalten eine Position und Masse eines zu simulierenden Körpers. Jeder Wurzelknoten eines Teilbaumes enthält einen Durchschnitt aller seiner Kindelemente. Die Masse entspricht der direkten Summe aller Massen. Und die Position die gewichtete Summe aller Positionen nach deren Masse.


Wir gehen von den n Prozessoren im maschinellem Modell aus. Die n-1 Prozessoren sind mit dem 0ten Prozess durch Kommunikationslinksverbunden. Der Prozess 0 teilt die Daten aus und sammelt die Teile der Berechneten Daten wieder ein. 

Es wird in unserem Fall nur ein paralleles Program L = (T, ET) auf n-1 Prozessoren verteilt, wobei die einzelnen Threads T lediglich mit dem Thread 0 auf Prozessor 0 kommunizieren sollen, ET besteht also nur aus den Kanten zu Prozess 0.  