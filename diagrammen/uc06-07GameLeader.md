## UC06_ReceivePostGameData

|UC06_ReceivePostGameData | |
|---|---|
| Actor | Laserguns |
| Doel | Het systeem van de gameleader ontvangt game data van laserguns om een eindstand te kunnen bepalen. |
| Pre-conditie | Er is nog maar 1 speler in het spel, of de speltijd is om. |
| Beschrijving | 1. Stuur ReadyForData signaal. <br/> 2. Ontvang game data per speler. <br/> 3. Ga naar UC07_AnalyzeAndDisplayStats. |
| Post-conditie | Alle data is binnen. |

<br/>

## UC07_AnalyzeAndDisplayStats

| UC07_AnalyzeAndDisplayStats | |
|---|---|
| Actor | Spelers, Gameleader |
| Doel | De data wordt geanalyseerd om de scores te bepalen en de winnaar uit te roepen. Ook wordt er gekozen of een nieuw spel wordt gespeeld met dezelfde of in nieuwe teams. |
| Pre-conditie | Alle data van de Laserguns is ontvangen. |
| Beschrijving | 1. Punten worden verdeeld op basis van: <br/> - Lives Left <br/> - Hits on players <br/> - Accuracy <br/>2. Display winnaar en stats<br/>3. Wacht tot de gameleader op "Same teams" of "New teams" drukt: <br/> - "Same teams" : Stuur signaal naar UC02_EnterGameSettings (bReset = false). <br/> - "New teams" : Stuur signaal naar UC02_EnterGameSettings (bReset = true). <br/>4. Ga naar UC02_EnterGameSettings. |
| Post-conditie | De spelers weten hun scores en de winnaar is bepaald, en een nieuw spel kan gestart worden.|


