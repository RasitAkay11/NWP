# Network Programming
## Algemene uitleg
Guessit is een spel die uitgevonden is door mij, Rasit Akay. In deze README file gaat er besproken worden hoe deze spel gerunt kan worden op ieder pc, maar ook hoe de algorithme in elkaar steekt.
Het spel is vrij simpel. Er zijn 6 spelers en 1 server. De server maakt een random getal aan tussen de 0 en 100. De clients moeten de random getal gokken, MAAR het hoeft niet per se raak te zijn! Diegene die het verste gok, wordt er uit gekickt.

## Technische uitleg
De game is gebaseerd op twee soorten commando's. Deze zijn Gok en Join. Wel is gok verdeeld over 6 clients, wat resulteert in 6 Gok commando's en één join commando. De commando's gaan zoals het volgende over de benternet:
### Server
- guessit>gok1?>
- guessit>gok2?>
- guessit>gok3?>
- guessit>gok4?>
- guessit>gok5?>
- guessit>gok6?>
- guessit>join?>
### Client
- Client 1:
1. guessit>gok1!>
2. guessit>join!>
- Client 2:
1. guessit>gok2!>
2. guessit>join!>
- Client 3:
1. guessit>gok3!>
2. guessit>join!>
- Client 4:
1. guessit>gok4!>
2. guessit>join!>
- Client 5:
1. guessit>gok5!>
2. guessit>join!>
- Client 6:
1. guessit>gok6!>
2. guessit>join!>

De server stuurt de berichten met een "?" waarop de clients antwoorden met een "!". De reden waarom elke client subscribed is op een andere gok topic, is zodat ze individueel kunnen communiceren met de server en ook andersom.

# Verloop game
1. Clients worden opgestart
2. Server wordt opgestart
- 3. LOOP
3.1. Server vraagt gok aan player I
3.2. Client ontvangt vraag
3.3. Client maakt antwoord
3.4. Client checkt of het antwoord tussen de 0 en 100 is
3.5. Client stuurt antwoord
3.6. Server ontvangt antwoord
Dit 6 keer, telkens bij verschillende clients
- 3. Einde LOOP
- 4. LOOP
4.1. Server neemt gok[i]
4.2. Server trekt daar het random getal van af om gelijk te stellen met de nul-as
Dit 6 keer, bij elke client
- 4. Einde LOOP
- 5. LOOP
5.1. Server detecteerd de hoogste
5.2. Server detecteerd de laagste
- 5. Einde LOOP
- 6. IF function
6.1 Server detecteert of de hoogste of de laagste het verste zit van de nul.
6.2 Server steekt die variabele in integer r.
- 6. Einde IF function
7. LOOP
7.1 Als integer i niet hetzelfde is aan integer r
7.1.1 Player[i] ontvangt "congratulations, you are not kicked!"
7.2 Else
7.2.1 Player[i] ontvangt "Sadly, you lost."
7. Einde LOOP
8. LOOP
8.1 integer i begint met integer r
8.2 Player die het verst heeft gegokt wordt gekickt door strcpy functie
8. Einde LOOP
