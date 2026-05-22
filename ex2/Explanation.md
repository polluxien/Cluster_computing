### Übung 2 Ansatz:

Für die Simulation der Massenpunkte verwendet man die Annahme, dass alle Massenpunkte ein und dieselbe Gravitationskonstante haben und den Newtonschen Gravitationsgesetz unterliegen, d.h. dass zwei Körper sich gegenseitig mit einer Kraft F_G anziehen. Die Kraft F_G ist proportional zum Produkt ihrer Massen und umgekehrt proportional zum Quadrat ihres Abstandes. Der Wert der Gravitationskonstante ist parammetrisierbar. Die Körper haben in unserem Modell keine räumliche Ausdehnung, wie angefordert und außerdem behandelt die Simulation keine Zusammenstöße einfachheitshalber, so dass die Körper aneinander vorbei „fliegen“. Ein Punkt hat 3 Eigenschaften: Position, Geschwindigkeit und die Beschleunigung. Wir führen eine Positionsberechnung für die Massenpunkte durch.  Die Beschleunigung wird nach dem Newton aus der Simulation berechnet. Neue Punktposition ist dann auf Basis derer berechnet.

Der initiale Datensatz ergibt aus eine randomisierten Erstellung in dem Worker 0. Wir gehen davon aus, dass der Seed standardmäßig in C konstant bleibt und somit der gleiche Datensatz entsteht.

Jeder Worker bekommt alle Punkte und berechnet eine Teilmenge davon. Der Worker 0 beschäftigt sich sich dazu mit der Verteilung und Zusammenfügung der Datenteile analog zu dem Ansatz in der vorherigen Übung zur Matrixmultiplikation.

Diese Aufteilung ist gewählt worden, denn die Teilprobleme innerhalb eines Zustandes unabhängig voneinander ist. Somit kann man die Randbedingung so formulieren, dass die Abhängigkeit der Berechnung innerhalb der Punkte nur zeitlich von der vorherigen Iterationsergebnis abhängen. Die Massenpunkte sind also paarweise unabhängig.



### Interpretation der Ergebnisse:

Wie viel schneller ist das Programm geworden. Wie groß ist die ursprüngliche Laufzeit. Wie ist der SpeedUp? Was ist der ScaleUp? Möglich? Kommunikationsaufwand noch in Rahmen?

SpeedUp nach Adams Law =>
 T(1) = Ts + Tp
f=Ts / (Ts + Tp) = 0,00000391
S(p) = 1 / (f + (1 -f/p) ) = 1/(0,00000391 + ((1- 0,00000391)/p))  das wäre eine Verbesserung von 9,99..%

Der Kommunikationsaufwand ist R/C ~ 37.75 und es ist noch viel höher als 10/2 = 5 somit kann man eigentlich die Prozessorenanzahl noch erhöhen.

Die Auslastung auf den Recheneinheiten ist, gezeigt durch die Laufzeitmessung auf die Einheiten mit geringerem Unterschied zu Prozess 0 gleich s. Bild. (die rechten Peeks) Die Kommunikationskosten haben geringeren Anteil von höchstens 0.03% gesamter Laufzeit mit 10 Kommunikationspartner.