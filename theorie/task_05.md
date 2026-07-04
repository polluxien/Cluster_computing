# Aufgabe 5-1 (Programm- und Kommunikationsgraphen)

##### Erläutern Sie den Prozesskommunikationsgraphen (TIG) und den Prozessvorgängergraphen (TPG).

##### Task-Interaktionsgraph (TIG)
Programgraph L=(T,E<sup>T</sup>), bei dem Kanten den Kommunikationsfluss zwischen Threads zeigen.
- Knoten haben Rechenaufwand β<sub>i</sub>, Kanten zeigen Kommunikationsintensität α<sub>ij</sub>
-  Räumliche Nähe kommunizierender Einheiten 

##### Task-Präzedenzgraph (TPG)
Gerichteter Graph. Kanten definieren Vorgänger-Nachfolger-Beziehungen (A muss vor B fertig sein).
- Knoten geben Ausführungszeit an, Kanten können Kommunikationsverzögerungen darstellen
- Zeitliche Abfolge von Berechnungsschritten

##### Zur Darstellung welchen Aspekts eines parallelen Programms würden Sie den Prozesskommunikationsgraphen (TIG) und für welchen Aspekt den Prozessvorgängergraphen (TPG) verwenden? Begründen Sie Ihre Antwort.


##### TIG für Kommunikationsoptimierung (Mapping/Partitioning)
Threads so auf Prozessoren verteilen, dass Kommunikations-Overhead minimal wird
- **Begründung**: Zeigt Datenaustauschintensität → Threads mit starker Kommunikation sollten auf gleichem/nachbar Prozessor laufen, um Latenz zu reduzieren
- **Typisch für**: Programme mit gleichzeitig existierenden Thread 

##### TPG für Zeitmanagement (Scheduling)
Minimierung der Gesamtlaufzeit bei abhängigen Threads
- **Begründung**: Einziger Graph, der logische Abhängigkeiten abbildet → notwendig für Scheduling-Algorithmen (wann startet welcher Thread?)
- **Typisch für**: Aufgaben mit Zwischenergebnissen (z.B. Gauss-Elimination, Sieb des Eratosthenes)