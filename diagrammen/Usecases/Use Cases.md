# Player

## Boot up / Initialize

### UC01_Connect
1. Connect with Leader Server/Broker
2. If no connection, display "No connection with Game Leader"
3. Go to UC02_PreGameSetup

## Pre Game

### UC02_GameSetup
1. Check if connected
2. Display "Waiting for Start"
3. Get Game Data
	1. Send get_data request
	2. Wait for data
4. Store game data
5. Respond to Setting Check from Leader
6. If fail, get game data again
7. Go to UC03_ReadyUp

### UC03_ReadyUp
1. Display "Press Trigger and Reload to Ready Up"
2. Send ready signal after reload and trigger press get registered
3. Set LEDs to team color
4. After Start Signal, Go to UC041_GameState

## In Game

### UC041_GameState
1. Update Game Timer every second
2. Display Time, Health, and Ammo
3. If Out of Health, go to UC05_GameOver
4. After Time Up, go to UC05_GameOver

### UC042_Shooting
1. Wait for trigger press
2. If ammo not 0, send shoot message
3. Play shoot sound
4. Increment shots taken
5. Wait for cooldown time
6. If ammo 0, wait for reload button press
	1. Wait for reload time
	2. Set ammo to max
7. go to 1

### UC043_ReceivingHits
1. wait for receive Hit Message
2. If Hit Message is from someone other than yourself and/or a player on another team, else go to 1
3. Store (Timestamp : Shooter)
4. Play hit Sound
5. Decrement Health
6. go to 1

## Post Game

### UC05_GameOver
1. Play Game Over sound
2. Disable shooting and getting hit
3. Send Game Over Message To Leader
4. Display "Game Over"
5. Wait for end of Game Time, then go to UC06_SendPostGameData

### UC06_SendPostGameData
1. Send Data to Leader Server
	1. Hits Registered with Timestamps
	2. Remaining Health
	3. Shots Taken
2. Go to UC02_GameSetup

---

# Game Leader

## Boot up / Initialize

### UC01_Setup
1. Start TCP Server/MQTT Broker
2. Start Logging
3. Go to UC02_EnterGameSettings

## Pre Game

### UC02_EnterGameSettings
1. Display possible settings
2. Enter setting parameters
3. On settings confirm, go to UC03_PlayerSetup

### UC03_PlayerSetup
1. Display "Waiting for Players"
2. Wait for connection from set amount of players
3. If all players connected, send response to received get_data requests
	1. Amount of lives
	2. Game length
	3. Assigned player number
	4. Assigned player team if selected
	5. Weapon damage
	6. Reload time
	7. (Available weapons)
4. Check if all players got settings
5. If all players newest settings, go to UC04_Start
6. If not, send settings to those players again, up to 3 times, then throw error

### UC04_Start
1. Wait for ready signal from all players
2. Display all ready players
3. If all players ready, send start signal
4. Go to UC051_GameTimer

## In Game

### UC051_GameTimer
1. Display Game timer
2. When game timer over, go to UC06_ReceivePostGameData

### UC052_PlayerStatus
1. Receive Game over from Players
2. Display Player Status
3. When only one player left, go to UC06_ReceivePostGameData

## Post Game

### UC06_ReceivePostGameData
1. Send ReadyForData signal
2. Receive game data per player (playerNum, registered_hits, lives_left, shots_taken)
3. Store in local struct temp_data_t
4. when all playerdata is in, for every player:
	1. count amount of hits a player has made
	2. calculate accuracy with shots_taken / hits_made
	3. add to PostGameData
5. Go to UC07_AnalyzeAndDisplayStats

### UC07_AnalyzeAndDisplayStats
1. Award points based on:
	1. Lives left
	2. Hits on players
	3. Accuracy
2. Display winner and stats
3. On button press "Same teams", send signal to UC02_EnterGameSettings bReset = false
4. On button press "New teams", send signal to UC02_EnterGameSettings bReset = true
5. Go to UC02_EnterGameSettings

