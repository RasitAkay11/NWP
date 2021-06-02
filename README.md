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
3. LOOP
4. Server vraagt gok aan player I
5. Client ontvangt vraag
6. Client maakt antwoord
7. Client checkt of het antwoord tussen de 0 en 100 is
8. Client stuurt antwoord
9. Server ontvangt antwoord
Dit 6 keer, telkens bij verschillende clients
10. LOOP
11. Server neemt gok[i]
12. Server trekt daar het random getal van af om gelijk te stellen met de nul-as
Dit 6 keer, bij elke client
13. LOOP
14. Server detecteerd de hoogste
15. Server detecteerd de laagste
16. IF function
17. Server detecteert of de hoogste of de laagste het verste zit van de nul.
18. Server steekt die variabele in integer r.
19. LOOP
20. Als integer i niet hetzelfde is aan integer r
21. Player[i] ontvangt "congratulations, you are not kicked!"
22. Else
23. Player[i] ontvangt "Sadly, you lost."
24. LOOP
25. integer i begint met integer r
26. Player die het verst heeft gegokt wordt gekickt door strcpy functie
27. Service stuurt join? commando naar elke client.
28. Service wacht op join!>"player x is back" commando van elke client
29. Game start opnieuw
