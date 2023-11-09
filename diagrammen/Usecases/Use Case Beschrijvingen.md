# Player
## UC01

| UC01_Connect   |                                                                                                  |
| -------------- | ------------------------------------------------------------------------------------------------ |
| Actor          | -                                                                       |
| Samenvatting   | Verbinden met de TCP server van de spelleider                                                    |
| Pre-conditie    | Gun wordt voor het eerst opgestart, of startsignaal wordt ontvangen.                             |
| Beschrijving   | 1. Connect with Leader Server/Broker <br> 2. Go to UC02_PreGameSetup                             |
| Post-conditie   | Verbinding met spelleider is opgezet                                                             | 
| Uitzonderingen | Als er geen verbinding gemaakt kan worden, wordt er gedisplayed "No connection with Game Leader" |


## UC02

| UC02_GameSetup |                                                                                                                                                                                                                                                |
| -------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Actor          | -                                                                                                                                                                                                                     |
| Samenvatting   | Ontvangen van spel instellingen                                                                                                                                                                                                                |
| Pre-conditie    | Startsignaal wordt ontvangen                                                                                                                                                                                                                   |
| Beschrijving   | 1. Check if connected <br>2. Display "Waiting for Start" <br>3. Get Game Data <br>&nbsp    1. Send get_data request <br>&nbsp    2. Wait for data <br>4. Store game data <br>5. Respond to Setting Check from Leader <br>6. Go to UC03_ReadyUp |
| Post-conditie   | Spel instellingen zijn opgeslagen en verwerkt                                                                                                                                                                                                  | 
| Uitzonderingen | Als setting check gefailed wordt (CRC/checksum komt niet overeen), go to 3                                                                                                                                                                     |

## UC03

| UC03_ReadyUp |                                                                                                                                                                                                      |
| ------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Actor        | Player                                                                                                                                                                                               | 
| Samenvatting | Wacht tot speler bevestiging geeft dat ze klaar zijn om te beginnen                                                                                                                                  |
| Pre-conditie  | Startsignaal wordt ontvangen                                                                                                                                                                         |
| Beschrijving | 1. Send ready signal after reload and trigger press get registered<br>2. After Start Signal, Go to UC041_GameState |
| Post-conditie | Speler staat klaar om te beginnen met het spel                                                                                                                                                       |

## UC041

| UC041_GameState | |
|---|---|
| Actor | - |
| Samenvatting | Update het scherm en checkt voor game over |
| Pre-conditie | UC03_ReadyUp moet klaar zijn |
| Beschrijving | 1.	Update Game Timer every second <br/> 2. Display Timer, Health, and Ammo <br/> 3.	If Out of Health, go to UC05_GameOver <br/> 4.	After Time Up, go to UC05_GameOver|
| Post-conditie | Ga naar UC042_GameOver |

## UC042

| UC042_Shooting | |
|---|---|
| Actor | player |
| Samenvatting | Wacht tot de trekker wordt ingedrukt, speelt vervolgens het schietgeluid af en vermindert de munitie met één. Als de munitie 0 is, wacht hij op herladen. |
| Pre-conditie | De trekker moet worden ingedrukt |
| Beschrijving | 1.	Wait for trigger press<br/>2.	If ammo not 0, send shoot message<br/>3.	Play shoot sound<br/>4.	Increment shots taken<br/>5.	Decrease ammo<br/>6.	Wait for cooldown time<br/>7. If ammo 0, wait for reload button press<br/>-	      Wait for reload time<br/>-	      Set ammo to max<br/>8.	go to 1|
| Post-conditie | n.v.t. |

## UC043

| UC043_RecievingHits | |
|---|---|
| Actor | - |
| Samenvatting | Wacht op het Hit-bericht en controleert vervolgens of het een vijand is. Zo ja, sla het op (tijdstempel: schieter), speel het hitgeluid af en verlaag de gezondheid |
| Pre-conditie | Hitbericht moet worden ontvangen |
| Beschrijving | 1.	wait for receive Hit Message<br/>2.	If Hit message is from someone other than you and/or a player on another team, else go to 1<br/>3.	Store (Timestamp : Shooter)<br/>4.	Play hit Sound<br/>5.	Decrement Health<br/>6.	go to 1|
| Post-conditie | n.v.t. |

## UC05

| UC05_GameOver | |
|---|---|
| Actor | Gameleader |
| Samenvatting | Na een speler zijn levens kwijt is of de speeltijd op is zal er verschillende functies uitgevoerd worden en dan door gestuurd worden naar UC06_SendPostGameData. |
| Pre-conditie | Komt uit GameState |
| Beschrijving | 1.	Play Game Over sound <br/> 2.	Disable shooting and getting hit <br/> 3.	Send Game Over Message to Leader <br/> 4.	Display “Game Over” <br/> 5. Count down until end of GameTime, then go to UC06_SendPostGameData.|
| Post-conditie | n.v.t. |

## UC06

| UC06_SendPostGameData | |
|---|---|
| Actor | - |
| Samenvatting | geeft de benodigde informatie door en wordt daarna doorgestuurd naar UC02_GameSetup. |
| Pre-conditie | Komt uit GameOver. |
| Beschrijving | 1. 1.	Send Data to Leader server. <br/> - 	Hits regesterd with Timestamps. <br/> - Remaining health. <br/> - Shots taken. <br/> 2. Go to UC02_GameSetup. |
| Post-conditie | n.v.t.|

# GameLeader
## UC01

| UC01_Setup | |
|---|---|
| Actor | GameLeader |
| Samenvatting |  |
| Pre-conditie |  |
| Beschrijving |  |
| Post-conditie |  |

## UC02

| UC02_EnterGameSettings | |
|---|---|
| Actor | GameLeader |
| Samenvatting |  |
| Pre-conditie |  |
| Beschrijving |  |
| Post-conditie |  |

## UC03

| UC03_PlayerSetup | |
|---|---|
| Actor | PlayerGun |
| Samenvatting |  |
| Pre-conditie |  |
| Beschrijving |  |
| Post-conditie |  |

## UC04

| UC04_Start   |                                                                                                                                                    |
| ------------ | -------------------------------------------------------------------------------------------------------------------------------------------------- |
| Actor        | GameLeader                                                                                                                                         |
| Samenvatting | Starten van lasergame spel                                                                                                                         |
| Pre-conditie  | Startsignaal wordt ontvangen                                                                                                                       |
| Beschrijving | 1. Wait for ready signal from all players<br>2. Display all ready players<br>3. If all players ready, send start signal<br>4. Go to UC051_GameTime |
| Post-conditie | Spel wordt gestart                                                                                                                                 | 

## UC051

| UC051_GameTimer |                                                                                  |
| --------------- | -------------------------------------------------------------------------------- |
| Actor           | GameLeader (intern systeem)                                                      |
| Samenvatting    | Bijhouden van de resterende tijd in het spel                                     |
| Pre-conditie     | Startsignaal wordt ontvangen                                                     |
| Beschrijving    | 1. Display Game timer<br>2. When game timer over, go to UC06_ReceivePostGameData |
| Post-conditie | n.v.t. |

## UC052

| UC052_PlayerStatus |                                                                                                                               |
| ------------------ | ----------------------------------------------------------------------------------------------------------------------------- |
| Actor              | GameLeader (intern systeem)                                                                                                   |
| Samenvatting       | Levend of dood status van spelers regelen                                                                                     |
| Pre-conditie        | Startsignaal wordt ontvangen                                                                                                  |
| Beschrijving       | 1. Receive Game over from Players<br>2. Display Player Status<br>3. When only one player left, go to UC06_ReceivePostGameData | 
| Post-conditie | n.v.t. |


## UC06

|UC06_ReceivePostGameData | |
|---|---|
| Actor | Laserguns |
| Samenvatting | Het systeem van de gameleader ontvangt game data van laserguns om een eindstand te kunnen bepalen. |
| Pre-conditie | Er is nog maar 1 speler in het spel, of de speltijd is om. |
| Beschrijving | 1. Stuur ReadyForData signaal. <br/> 2. Ontvang game data per speler. <br/> 3. Ga naar UC07_AnalyzeAndDisplayStats. |
| Post-conditie | Alle data is binnen. |

## UC07

| UC07_AnalyzeAndDisplayStats | |
|---|---|
| Actor | Spelers, Gameleader |
| Samenvatting | De data wordt geanalyseerd om de scores te bepalen en de winnaar uit te roepen. Ook wordt er gekozen of een nieuw spel wordt gespeeld met dezelfde of in nieuwe teams. |
| Pre-conditie | Alle data van de Laserguns is ontvangen. |
| Beschrijving | 1. Punten worden verdeeld op basis van: <br/> - Lives Left <br/> - Hits on players <br/> - Accuracy <br/>2. Display winnaar en stats<br/>3. Wacht tot de gameleader op "Same teams" of "New teams" drukt: <br/> - "Same teams" : Stuur signaal naar UC02_EnterGameSettings (bReset = false). <br/> - "New teams" : Stuur signaal naar UC02_EnterGameSettings (bReset = true). <br/>4. Ga naar UC02_EnterGameSettings. |
| Post-conditie | De spelers weten hun scores en de winnaar is bepaald, en een nieuw spel kan gestart worden.|
