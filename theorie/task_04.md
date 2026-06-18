# Aufgabe 4-1 (Heuristische Suchverfahren)

##### Die Grundlage für die Benutzung von Suchheuristiken ist die lokale, minimale Änderung in Elementarschritten. Wie können diese minimalen Änderungen (Elementarschritte) für die in der Vorlesung vorgestellten Beispiele von Zuordnungsproblemen aussehen?

- Arbeiten mit kleinen, lokalen Änderungen an einer Lösung → Schritt für Schritt bessere Ergebnisse
- Diese kleinen Schritte = **Elementarschritte**
- Abhängig von der Problemdarstellung (meist String aus festen Einheiten)
---
 
##### 1. Quantitative Partitionierung
 
- Mehrere Prozessoren auf verschiedene Programme verteilt
- Gesamtanzahl bleibt gleich
- **Elementarschritt:** Ein Programm −1 Prozessor, ein anderes +1 → Prozessor verschoben, Summe bleibt erhalten
##### 2. Qualitative Partitionierung
 
- Einzelne Prozessoren bestimmten Programmen zugeordnet
- **Elementarschritt:** Prozessor anderem Programm zuweisen oder freigeben → ändert nur einen Wert im String
##### 3. Kontraktive Zuordnung (Mapping)
 
- Mehrere Threads pro Prozessor möglich, jeder Thread hat genau einen Prozessor
- **Elementarschritt:** Thread auf anderen Prozessor verschieben
  - nur benachbarter Prozessor → lokale Suche
  - beliebiger Prozessor → globale Suche
##### 4. Injektive Zuordnung
 
- Jeder Thread genau ein Prozessor, keine zwei Threads auf demselben
- **Elementarschritt:** Zwei Threads tauschen Prozessoren ODER Thread auf freien Prozessor verschieben
##### 5. Lastausgleich (Load Balancing)
 
- Ziel: Arbeit (Threads) gleichmäßig auf alle Prozessoren verteilen
- **Elementarschritt:** Thread von überlastetem → weniger belasteten Prozessor verschieben
---
 
##### Fazit
 
- Art des Elementarschritts hängt meist codierung ab
- Feste Gesamtsumme (Prozessoren/Threads) → meist verschieben/shift
- Zuordnung ohne feste Summe → oft nur lokale Wertänderung
- Topologien (z. B. Netzwerk) → manchmal nur Wechsel zu benachbarten Knoten