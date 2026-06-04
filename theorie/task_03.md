# Aufgabe 3-1 (Speed-up)

##### a) Diskutieren Sie das Verhältnis von Rechenzeit zur Kommunikationszeit eines parallelen Programms. Welche Komponenten, die das Zuordnungsproblem beschreiben, spielen hierbei eine maßgebliche Rolle? Begründen Sie Ihre Antwort.

Die Gesamtausführungszeit eines parallelen Programms setzt sich aus zwei Komponenten zusammen:

**T(p) = T~x~(p) + T~c~(p)**

| Komponente                   | Beschreibung                                                                     |
| ---------------------------- | -------------------------------------------------------------------------------- |
| **T~x~(p) - Execution**         | Sinkt typischerweise mit mehr Prozessoren, da die Last verteilt wird             |
| **T~c~(p) - Communication** | Steigt mit mehr Prozessoren → Koordination und Datenaustausch werden aufwendiger |

Ab einer bestimmten Prozessorzahl kann die Gesamtlaufzeit wieder ansteigen, wenn der Kommunikations-Overhead den Gewinn übersteigt. Parallelisierung lohnt sich nur, wenn das Verhältnis von Rechenzeit zu Kommunikationszeit (R/C) ausreichend groß ist.



Um dieses Verhältnis zu optimieren, sind vier Elemente entscheidend:

1. **Lastmodell - L (Load Model):** 
beschreibt die Strucktur von Threads und deren Interaktionsbeziehungen. Definiert die inhärente Granularität der Anwendung

2. **Maschinenmodell - M (Machine Model):** 
beschreibt Prozessoren und ihre Vernetzung. Die Übertragungsgeschwindigkeiten der Verbindungen bestimmen tatsächliche Kommunikationszeit

3. **Zuordnungsrelation - R (Allocation Relation):** 
bestimmt, welche Threads auf welchen Prozessoren laufen. Durch geschicktes Mapping (z.B. Clustering) können stark interagierende Threads demselben Prozessor zugeordnet werden → dann fallen die Kommunikationskosten praktisch weg.

4. **Zuordnungsziel - G (Allocation Goal):** 
definiert Optimierungsrichtung: Minimierung der Antwortzeit oder der Kommunikationskosten. Steuert den Kompromiss zwischen Lastausgleich (mehr Prozessoren) und Overhead-Vermeidung (weniger Prozessoren oder lokales Clustering).

---
##### Quellen
- *Cluster Computing SoSe 2025* – Chapter 3: Performance Aspects
- *Cluster Computing SoSe 2025* – Chapter 4: Allocation Problems in Parallel Computers
