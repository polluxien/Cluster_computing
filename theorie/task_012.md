# Aufgabe 2-1 (Speed-up)

##### a) Welche Aussagen lassen sich aus Amdahls Gesetz ableiten? Welcher andere Ansatz der Untersuchung paralleler Programme tritt Amdahls Gesetz entgegen? Erläutern Sie jeweils ein Beispiel für jeden dieser Ansätze.

**Amdahls Gesetz:**
- Unterteilt Ausführungszeit in sequenziellen Anteil (Ts) und parallelisierbaren Anteil (Tp)
- Speed-up ist begrenzt durch den sequenziellen Teil – Geschwindigkeitszuwachs beschränkt sich auf parallelisierbare Anteile
- Ausführungszeit kann niemals kürzer sein als die Zeit für den sequenziellen Teil
- Mit zunehmender Prozessorzahl sinkt die Effizienz E(p) und nähert sich bei unendlich vielen Prozessoren dem Wert 0

**Gegenansatz: Gustafson-Gesetz (Skalierungsgesetz)**
- Fokussiert sich auf skalierende Problemgröße statt fester Problemgröße
- Fragestellung: „Wie viel größer kann ein Problem sein und dennoch in gleicher Zeit von p Prozessoren gelöst werden?"
- Effizienz steigt, wenn Problemgröße proportional zur Prozessoranzahl erhöht wird
- System gilt als skalierbar, wenn Leistung bei gleichzeitiger Erhöhung von Problemgröße und Prozessoranzahl steigt

**Beispiele:**
- *Amdahl:* Programm mit 5 % sequenziellem Anteil (f=0,05) – Speed-up kann niemals 20 (1/0,05) überschreiten, Kurve flacht mit mehr Prozessoren ab
- *Gustafson (Scale-up):* Parallele Summation – bei fester Problemgröße (n=64) flacht Speed-up ab, bei proportionaler Erhöhung (n=512 bei 32 Prozessoren) näher an linearer Skalierung → zeigt Isoeffizienz-Prinzip

##### b) Gegeben seien zwei Programme P1 und P2 mit ihren durch Messung ermittelten Speed-up-Werten S1(p) bzw. S2(p)

*Wie viele Prozessoren würden Sie den Programmen jeweils zuordnen, wenn Sie insgesamt p=15 Prozessoren zur Verfügung haben? Begründen Sie Ihre Entscheidung. Wie wäre Ihre Aufteilung bei p=5, p=10, p=20 Prozessoren?*

| p     | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | 11  | 12  |
| ----- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| S1(p) | 1,0 | 1,5 | 2,0 | 2,5 | 3,0 | 3,5 | 4,0 | 4,5 | 5,0 | 5,5 | 6,0 | 6,5 |
| S2(p) | 1,0 | 1,9 | 2,7 | 3,4 | 4,0 | 4,3 | 4,5 | 4,6 | 4,7 | 4,7 | 4,7 | 4,7 |

**Empfehlung für p = 15:**
- **P2:** 9 Prozessoren → Speed-up stagniert ab p=9 bei 4,7, weitere Prozessoren nicht mehr so einflussreich
- **P1:** 6 Prozessoren → konstantes linearer Speed-up von 3,0 
- **Summenspeed-up:** 4,7 + 3,0 = 7,7 (bei 9+6) vs. 4,6 + 3,5 = 8,1 (bei 8+7) → je nach Optimierungskriterium variierend

p = 5: S1(1), S2(4) → 3,4 + 1,0 = 4,4
p = 10: S1(5), S2(5) → 3,0 + 4,0 = 7
p = 15: S1(10), S2(5) → 5,5 + 4,0 = 9,5
p = 20: S1(15), S2(5) → 8,0 + 4,0 = 12

