# Player
## UC01

| UC01_Connect   |                                                                                                  |
| -------------- | ------------------------------------------------------------------------------------------------ |
| Actor          | PlayerGun (intern systeem)                                                                       |
| Samenvatting   | Verbinden met de TCP server van de spelleider                                                    |
| Preconditie    | Gun wordt voor het eerst opgestart, of startsignaal wordt ontvangen.                             |
| Beschrijving   | 1. Connect with Leader Server/Broker <br> 2. Go to UC02_PreGameSetup                             |
| Postconditie   | Verbinding met spelleider is opgezet                                                             | 
| Uitzonderingen | Als er geen verbinding gemaakt kan worden, wordt er gedisplayed "No connection with Game Leader" |


## UC02

| UC02_GameSetup |                                                                                                                                                                                                                                                |
| -------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Actor          | PlayerGun (intern systeem)                                                                                                                                                                                                                     |
| Samenvatting   | Ontvangen van spel instellingen                                                                                                                                                                                                                |
| Preconditie    | Startsignaal wordt ontvangen                                                                                                                                                                                                                   |
| Beschrijving   | 1. Check if connected <br>2. Display "Waiting for Start" <br>3. Get Game Data <br>&nbsp    1. Send get_data request <br>&nbsp    2. Wait for data <br>4. Store game data <br>5. Respond to Setting Check from Leader <br>6. Go to UC03_ReadyUp |
| Postconditie   | Spel instellingen zijn opgeslagen en verwerkt                                                                                                                                                                                                  | 
| Uitzonderingen | Als setting check gefailed wordt (CRC/checksum komt niet overeen), go to 3                                                                                                                                                                     |

## UC03

| UC03_ReadyUp |                                                                                                                                                                                                      |
| ------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Actor        | Player                                                                                                                                                                                               | 
| Samenvatting | Wacht tot speler bevestiging geeft dat ze klaar zijn om te beginnen                                                                                                                                  |
| Preconditie  | Startsignaal wordt ontvangen                                                                                                                                                                         |
| Beschrijving | 1. Display "Press Trigger and Reload to Ready Up"<br>2. Send ready signal after reload and trigger press get registered<br>3. Set LEDs to team color<br>4. After Start Signal, Go to UC041_GameState |
| Postconditie | Speler staat klaar om te beginnen met het spel                                                                                                                                                       |

# GameLeader
## UC04

| UC04_Start   |                                                                                                                                                    |
| ------------ | -------------------------------------------------------------------------------------------------------------------------------------------------- |
| Actor        | GameLeader                                                                                                                                         |
| Samenvatting | Starten van lasergame spel                                                                                                                         |
| Preconditie  | Startsignaal wordt ontvangen                                                                                                                       |
| Beschrijving | 1. Wait for ready signal from all players<br>2. Display all ready players<br>3. If all players ready, send start signal<br>4. Go to UC051_GameTime |
| Postconditie | Spel wordt gestart                                                                                                                                 | 

## UC051

| UC051_GameTimer |                                                                                  |
| --------------- | -------------------------------------------------------------------------------- |
| Actor           | GameLeader (intern systeem)                                                      |
| Samenvatting    | Bijhouden van de resterende tijd in het spel                                     |
| Preconditie     | Startsignaal wordt ontvangen                                                     |
| Beschrijving    | 1. Display Game timer<br>2. When game timer over, go to UC06_ReceivePostGameData |

## UC052

| UC052_PlayerStatus |                                                                                                                               |
| ------------------ | ----------------------------------------------------------------------------------------------------------------------------- |
| Actor              | GameLeader (intern systeem)                                                                                                   |
| Samenvatting       | Levend of dood status van spelers regelen                                                                                     |
| Preconditie        | Startsignaal wordt ontvangen                                                                                                  |
| Beschrijving       | 1. Receive Game over from Players<br>2. Display Player Status<br>3. When only one player left, go to UC06_ReceivePostGameData | 
