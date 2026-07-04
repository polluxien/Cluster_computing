## Idee / Architektur

Das Projekt wird als eine Reihe von MPI-Prozessen gestartet.
Jeder MPI-Prozess hat in einem Array hinterlegt, wie viele OpenMP Threads er erzeugen soll. Gewichtet nach der Anzahl von Threads wird die Liste aller Punktmassen auf die MPI Prozesse verteilt. Diese verteilen ihr Intervall gleichmäßig auf Ihre OpenMP Threads.


## Probleme
Bei manchen Konfigurationen von Prozessanzahl, Threadanzahl und Anzahl von Körpern führt die Ausführung des Programms zum Festhängen. Dabei gibt es keine offensichtlichen Errors wie Segmentation Faults oder Ähnliches. Fachlich stimmt das Programm, da das Punktinterval korrekt nach Gewichtung aufgeteilt wird und jeder Thread davon ein gleichgroßes Stück bekommt. Auch die Berechnungen wurden manuell überprüft.

## Verbesserung
Gepalnt wird eine Verbesserung in dem kommenden Wochenende zu ermöglichen. Die Verbesserungsvorschläge findet man in der ausbild.pdf Präsentation vor.