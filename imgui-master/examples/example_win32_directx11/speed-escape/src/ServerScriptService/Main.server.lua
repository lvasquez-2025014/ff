-- Script principal del servidor: estado, remotes, checkpoints, tienda, misiones y prestige.
local Players = game:GetService("Players")
local ReplicatedStorage = game:GetService("ReplicatedStorage")
local MarketplaceService = game:GetService("MarketplaceService")

local DataService = require(script.Parent:WaitForChild("DataService"))
local MapBuilder = require(script.Parent:WaitForChild("MapBuilder"))
local Config = require(ReplicatedStorage:WaitForChild("Shared"):WaitForChild("Config"))

local Remotes = ReplicatedStorage:FindFirstChild("Remotes")
if not Remotes then
	Remotes = Instance.new("Folder")
	Remotes.Name = "Remotes"
	Remotes.Parent = ReplicatedStorage
end

local function ensureRemote(className, name)
	local remote = Remotes:FindFirstChild(name)
	if not remote then
		remote = Instance.new(className)
		remote.Name = name
		remote.Parent = Remotes
	end
	return remote
end

local AddSpeed = ensureRemote("RemoteEvent", "AddSpeed")
local Rebirth = ensureRemote("RemoteEvent", "Rebirth")
local BuyUpgrade = ensureRemote("RemoteEvent", "BuyUpgrade")
local SyncState = ensureRemote("RemoteEvent", "SyncState")
local Notify = ensureRemote("RemoteEvent", "Notify")
local GetState = ensureRemote("RemoteFunction", "GetState")
local FX = ensureRemote("RemoteEvent", "FX")
local Leaderboard = ensureRemote("RemoteEvent", "Leaderboard")
local ClaimDaily = ensureRemote("RemoteEvent", "ClaimDaily")

local mapData = MapBuilder.Build()
local spawnLocation = mapData.spawn
local checkpoints = mapData.checkpoints
local coins = mapData.coins
local mapFolder = workspace:FindFirstChild("SpeedEscapeMap") or workspace
local HEIST_ROUND_DURATION = 120
local heistRoundEndsAt = workspace:GetServerTimeNow() + HEIST_ROUND_DURATION

local heistCores = {}
local coreColors = {
	Color3.fromRGB(80, 220, 255),
	Color3.fromRGB(255, 90, 220),
	Color3.fromRGB(255, 215, 60),
}
for index = 1, Config.HeistCoreCount do
	local core = Instance.new("Part")
	core.Name = "HeistCore_" .. index
	core.Shape = Enum.PartType.Ball
	core.Size = Vector3.new(5, 5, 5)
	core.Position = Vector3.new(0, 6, -math.floor(Config.TrackLength * (index / (Config.HeistCoreCount + 1))))
	core.Material = Enum.Material.Neon
	core.Color = coreColors[index] or Color3.fromRGB(255, 215, 60)
	core.CanCollide = false
	core.CanTouch = true
	core.Parent = mapFolder
	local pedestal = Instance.new("Part")
	pedestal.Name = "HeistCorePedestal_" .. index
	pedestal.Anchored = true
	pedestal.Size = Vector3.new(7, 0.35, 7)
	pedestal.Position = Vector3.new(core.Position.X, 1.2, core.Position.Z)
	pedestal.Material = Enum.Material.Metal
	pedestal.Color = Color3.fromRGB(20, 25, 52)
	pedestal.CanCollide = false
	pedestal.Parent = mapFolder
	local beam = Instance.new("Part")
	beam.Name = "HeistCoreBeam_" .. index
	beam.Anchored = true
	beam.Size = Vector3.new(0.35, 9, 0.35)
	beam.Position = Vector3.new(core.Position.X, 5.4, core.Position.Z)
	beam.Material = Enum.Material.Neon
	beam.Color = core.Color
	beam.Transparency = 0.35
	beam.CanCollide = false
	beam.Parent = mapFolder
	local corePlatform = Instance.new("Part")
	corePlatform.Name = "HeistCorePlatform_" .. index
	corePlatform.Anchored = true
	corePlatform.Size = Vector3.new(28, 0.3, 22)
	corePlatform.Position = Vector3.new(0, 1.05, core.Position.Z)
	corePlatform.Material = Enum.Material.Metal
	corePlatform.Color = Color3.fromRGB(16, 22, 48)
	corePlatform.CanCollide = false
	corePlatform.Parent = mapFolder
	for _, edge in ipairs({
		{ Vector3.new(28, 0.18, 0.55), Vector3.new(0, 1.3, core.Position.Z - 11) },
		{ Vector3.new(28, 0.18, 0.55), Vector3.new(0, 1.3, core.Position.Z + 11) },
		{ Vector3.new(0.55, 0.18, 22), Vector3.new(-14, 1.3, core.Position.Z) },
		{ Vector3.new(0.55, 0.18, 22), Vector3.new(14, 1.3, core.Position.Z) },
	}) do
		local edgePart = Instance.new("Part")
		edgePart.Name = "HeistCoreEdge_" .. index
		edgePart.Anchored = true
		edgePart.Size = edge[1]
		edgePart.Position = edge[2]
		edgePart.Material = Enum.Material.Neon
		edgePart.Color = core.Color
		edgePart.Transparency = 0.18
		edgePart.CanCollide = false
		edgePart.Parent = mapFolder
	end
	for segment = 1, 12 do
		local angle = ((segment - 1) / 12) * math.pi * 2
		local ring = Instance.new("Part")
		ring.Name = "HeistCoreRing_" .. index
		ring.Anchored = true
		ring.Size = Vector3.new(3.8, 0.18, 0.5)
		ring.Position = Vector3.new(math.cos(angle) * 7, 1.55, core.Position.Z + math.sin(angle) * 7)
		ring.CFrame = CFrame.new(ring.Position) * CFrame.Angles(0, -angle, 0)
		ring.Material = Enum.Material.Neon
		ring.Color = core.Color
		ring.CanCollide = false
		ring.Parent = mapFolder
	end
	local light = Instance.new("PointLight")
	light.Color = core.Color
	light.Brightness = 3
	light.Range = 18
	light.Parent = core
	local highlight = Instance.new("Highlight")
	highlight.Adornee = core
	highlight.FillColor = core.Color
	highlight.FillTransparency = 0.72
	highlight.OutlineColor = core.Color
	highlight.OutlineTransparency = 0.05
	highlight.DepthMode = Enum.HighlightDepthMode.AlwaysOnTop
	highlight.Parent = core
	local particles = Instance.new("ParticleEmitter")
	particles.Rate = 8
	particles.Lifetime = NumberRange.new(0.7, 1.3)
	particles.Speed = NumberRange.new(0.4, 1.2)
	particles.SpreadAngle = Vector2.new(360, 360)
	particles.Size = NumberSequence.new({
		NumberSequenceKeypoint.new(0, 0.22),
		NumberSequenceKeypoint.new(1, 0),
	})
	particles.LightEmission = 1
	particles.Color = ColorSequence.new(core.Color)
	particles.Parent = core
	local label = Instance.new("BillboardGui")
	label.Size = UDim2.fromOffset(150, 36)
	label.StudsOffset = Vector3.new(0, 4.5, 0)
	label.AlwaysOnTop = true
	label.LightInfluence = 0
	label.Parent = core
	local text = Instance.new("TextLabel")
	text.Size = UDim2.fromScale(1, 1)
	text.BackgroundTransparency = 1
	text.Text = "NUCLEO " .. index .. "/" .. Config.HeistCoreCount
	text.TextColor3 = core.Color
	text.TextStrokeTransparency = 0.2
	text.Font = Enum.Font.GothamBlack
	text.TextScaled = true
	text.Parent = label
	heistCores[index] = core
end

task.spawn(function()
	while true do
		task.wait(1 / 30)
		for _, core in ipairs(heistCores) do
			if core and core.Parent then
				core.CFrame *= CFrame.Angles(0, 0.035, 0)
			end
		end
	end
end)

local passCache = {}
local keyBuckets = {}
local comboState = {}
local boostCooldowns = {}
local obstacleCooldowns = {}
local finishCooldowns = {}
local notify
local updateBest

local function snapshot(data)
	local upgrades = {}
	for key in pairs(Config.Upgrades) do
		upgrades[key] = data.Upgrades[key] or 0
	end
	local missions = {}
	for key, mission in pairs(data.Missions) do
		missions[key] = { Level = mission.Level, Progress = mission.Progress }
	end
	return {
		Speed = data.Speed,
		Coins = data.Coins,
		Rebirths = data.Rebirths,
		Checkpoint = data.Checkpoint,
		Best = data.Best or 0,
		DailyStreak = data.DailyStreak or 0,
		LastDailyClaim = data.LastDailyClaim or "",
		HeistCores = data.HeistCores or 0,
		HeistGoal = Config.HeistCoreCount,
		HeistRoundEndsAt = heistRoundEndsAt,
		Upgrades = upgrades,
		Missions = missions,
	}
end

local function utcDay(offset)
	return os.date("!%Y-%m-%d", os.time() + (offset or 0))
end

local function claimDailyReward(player)
	local data = DataService.Get(player)
	if not data then
		return
	end
	local today = utcDay()
	if data.LastDailyClaim == today then
		notify(player, "Ya reclamaste tu bono diario. Vuelve mañana.")
		return
	end
	local yesterday = utcDay(-86400)
	if data.LastDailyClaim ~= yesterday then
		data.DailyStreak = 0
	end
	data.DailyStreak = math.clamp((data.DailyStreak or 0) + 1, 1, #Config.DailyRewards)
	data.LastDailyClaim = today
	local reward = Config.DailyRewards[data.DailyStreak] or Config.DailyRewards[#Config.DailyRewards]
	local speedReward = Config.DailySpeedReward * data.DailyStreak
	data.Coins += reward
	data.Speed += speedReward
	updateBest(data)
	notify(player, string.format("BONO DIARIO %d/7: +%d monedas y +%d velocidad.", data.DailyStreak, reward, speedReward))
	FX:FireClient(player, "Daily")
	SyncState:FireClient(player, snapshot(data))
end

notify = function(player, message)
	Notify:FireClient(player, message)
end

updateBest = function(data)
	if data.Speed > (data.Best or 0) then
		data.Best = data.Speed
	end
end

local function currentCombo(player)
	local state = comboState[player]
	if not state then
		return 0
	end
	if os.clock() - state.Last > Config.ComboWindow then
		return 0
	end
	return state.Count
end

local function hasPass(player, key)
	local id = Config.PassIds[key]
	if not id or id == 0 then
		return false
	end
	local cached = passCache[player]
	if cached and cached[key] ~= nil then
		return cached[key]
	end
	local ok, owns = pcall(function()
		return MarketplaceService:UserOwnsGamePassAsync(player.UserId, id)
	end)
	owns = ok and owns == true
	passCache[player] = cached or {}
	passCache[player][key] = owns
	return owns
end

local function passOwned(player, key)
	local cached = passCache[player]
	return cached ~= nil and cached[key] == true
end

local function coinMultiplier(player, data)
	local multiplier = 1 + data.Rebirths * Config.RebirthCoinBonus
	multiplier *= 1 + data.Upgrades.Coins * Config.Upgrades.Coins.Effect
	if passOwned(player, "DoubleCoins") then
		multiplier *= 2
	end
	return multiplier
end

local function applyHumanoid(player, data)
	local character = player.Character
	if not character then
		return
	end
	local humanoid = character:FindFirstChildOfClass("Humanoid")
	if not humanoid then
		return
	end
	humanoid.WalkSpeed = Config.GetWalkSpeed(
		data.Speed,
		data.Rebirths,
		data.Upgrades.Speed,
		passOwned(player, "DoubleSpeed")
	)
	humanoid.UseJumpPower = true
	humanoid.JumpPower = 50 + data.Upgrades.Jump * Config.Upgrades.Jump.Effect
end

local function respawnCFrame(index)
	if index and index > 0 and checkpoints[index] then
		return checkpoints[index].CFrame + Vector3.new(0, 6, 0)
	end
	return spawnLocation.CFrame + Vector3.new(0, 5, 0)
end

local function teleportToCheckpoint(player)
	local data = DataService.Get(player)
	if not data then
		return
	end
	local character = player.Character
	if not character then
		return
	end
	local root = character:FindFirstChild("HumanoidRootPart")
	if not root then
		return
	end
	local target = respawnCFrame(data.Checkpoint)
	character:PivotTo(CFrame.lookAt(target.Position, target.Position + Vector3.new(0, 0, -1)))
	root.AssemblyLinearVelocity = Vector3.zero
end

task.spawn(function()
	while true do
		task.wait(1)
		if workspace:GetServerTimeNow() >= heistRoundEndsAt then
			heistRoundEndsAt = workspace:GetServerTimeNow() + HEIST_ROUND_DURATION
			for _, player in ipairs(Players:GetPlayers()) do
				local data = DataService.Get(player)
				if data then
					data.HeistCores = 0
					data.Checkpoint = 0
					teleportToCheckpoint(player)
					notify(player, "NUEVA RONDA. Recupera 3 nucleos antes de que se acabe el tiempo.")
					FX:FireClient(player, "RoundReset")
					SyncState:FireClient(player, snapshot(data))
				end
			end
		end
	end
end)

local function setupCharacter(player, character)
	task.wait(0.15)
	if player.Character ~= character then
		return
	end
	teleportToCheckpoint(player)
end

local function checkMissions(player, data, key)
	local mission = data.Missions[key]
	local definition = Config.Missions[key]
	if not mission or not definition then
		return
	end
	local completed = false
	for _ = 1, 25 do
		local target = Config.GetMissionTarget(key, mission.Level)
		if mission.Progress < target then
			break
		end
		local reward = Config.GetMissionReward(key, mission.Level)
		mission.Progress -= target
		mission.Level += 1
		data.Coins += reward
		completed = true
		notify(player, string.format("MISION: %s %d. +%d monedas.", definition.Label, mission.Level, reward))
		FX:FireClient(player, "Mision")
	end
	if completed then
		SyncState:FireClient(player, snapshot(data))
	end
end

local function setupLeaderstats(player, data)
	local folder = Instance.new("Folder")
	folder.Name = "leaderstats"

	local speed = Instance.new("IntValue")
	speed.Name = "Speed"
	speed.Value = math.floor(data.Speed)

	local coinsValue = Instance.new("IntValue")
	coinsValue.Name = "Coins"
	coinsValue.Value = math.floor(data.Coins)

	local rebirths = Instance.new("IntValue")
	rebirths.Name = "Rebirths"
	rebirths.Value = data.Rebirths

	local best = Instance.new("IntValue")
	best.Name = "Best"
	best.Value = math.floor(data.Best or 0)

	speed.Parent = folder
	coinsValue.Parent = folder
	rebirths.Parent = folder
	best.Parent = folder
	folder.Parent = player
end

local function onPlayerAdded(player)
	if player:GetAttribute("SpeedEscapeLoaded") then
		return
	end
	player:SetAttribute("SpeedEscapeLoaded", true)

	local data = DataService.Load(player)
	if not player.Parent then
		DataService.Release(player)
		return
	end

	updateBest(data)
	setupLeaderstats(player, data)
	SyncState:FireClient(player, snapshot(data))
	notify(player, "Presiona teclas para +1 velocidad. Corre con WASD.")

	task.spawn(function()
		hasPass(player, "DoubleSpeed")
		hasPass(player, "DoubleCoins")
	end)

	player.CharacterAdded:Connect(function(character)
		task.spawn(setupCharacter, player, character)
	end)
	if player.Character then
		task.spawn(setupCharacter, player, player.Character)
	end
end

ClaimDaily.OnServerEvent:Connect(claimDailyReward)

Players.PlayerAdded:Connect(function(player)
	task.spawn(onPlayerAdded, player)
end)

for _, player in ipairs(Players:GetPlayers()) do
	task.spawn(onPlayerAdded, player)
end

AddSpeed.OnServerEvent:Connect(function(player)
	local data = DataService.Get(player)
	if not data then
		return
	end
	local now = os.clock()
	local bucket = keyBuckets[player]
	if not bucket then
		bucket = { tokens = Config.KeyRateLimit, last = now }
		keyBuckets[player] = bucket
	end
	bucket.tokens = math.min(Config.KeyRateLimit, bucket.tokens + (now - bucket.last) * Config.KeyRateLimit)
	bucket.last = now
	if bucket.tokens < 1 then
		return
	end
	bucket.tokens -= 1

	local combo = comboState[player]
	if not combo then
		combo = { Count = 0, Last = 0 }
		comboState[player] = combo
	end
	if now - combo.Last <= Config.ComboWindow then
		combo.Count = math.min(Config.ComboMax, combo.Count + 1)
	else
		combo.Count = 1
	end
	combo.Last = now

	data.Speed += 1
	updateBest(data)
	data.Missions.Keys.Progress += 1
	checkMissions(player, data, "Keys")
end)

GetState.OnServerInvoke = function(player)
	local data = DataService.Get(player)
	if not data then
		return nil
	end
	return snapshot(data)
end

Rebirth.OnServerEvent:Connect(function(player)
	local data = DataService.Get(player)
	if not data then
		return
	end
	if data.Speed < Config.RebirthRequirement then
		notify(player, string.format(
			"Necesitas %d de velocidad para renacer. Tienes %d.",
			Config.RebirthRequirement,
			math.floor(data.Speed)
		))
		return
	end
	data.Rebirths += 1
	data.Speed = 0
	data.Checkpoint = 0
	data.HeistCores = 0
	local combo = comboState[player]
	if combo then
		combo.Count = 0
	end
	teleportToCheckpoint(player)
	SyncState:FireClient(player, snapshot(data))
	FX:FireClient(player, "Renacer")
	notify(player, string.format(
		"RENACIMIENTO %d. Bonus: +%d%% velocidad y monedas.",
		data.Rebirths,
		math.floor(data.Rebirths * Config.RebirthSpeedBonus * 100)
	))
end)

BuyUpgrade.OnServerEvent:Connect(function(player, upgradeKey)
	if type(upgradeKey) ~= "string" then
		return
	end
	local data = DataService.Get(player)
	if not data then
		return
	end
	local upgrade = Config.Upgrades[upgradeKey]
	if not upgrade then
		return
	end
	local level = data.Upgrades[upgradeKey] or 0
	if level >= upgrade.MaxLevel then
		notify(player, "Esa mejora ya esta al maximo.")
		return
	end
	local cost = Config.GetUpgradeCost(upgradeKey, level)
	if data.Coins < cost then
		notify(player, string.format("Te faltan %d monedas.", cost - data.Coins))
		return
	end
	data.Coins -= cost
	data.Upgrades[upgradeKey] = level + 1
	SyncState:FireClient(player, snapshot(data))
	notify(player, string.format("%s al nivel %d.", upgrade.Label, level + 1))
end)

for index, part in ipairs(checkpoints) do
	part.Touched:Connect(function(hit)
		local player = Players:GetPlayerFromCharacter(hit.Parent)
		if not player then
			return
		end
		local data = DataService.Get(player)
		if not data or index <= data.Checkpoint then
			return
		end
		data.Checkpoint = index
		data.Speed += Config.CheckpointSpeedBonus
		updateBest(data)
		local reward = math.floor(
			Config.CheckpointCoinBase
				* index
				* coinMultiplier(player, data)
				* Config.GetComboMultiplier(currentCombo(player))
		)
		data.Coins += reward
		data.Missions.Checkpoint.Progress += 1
		notify(player, string.format(
			"Checkpoint %d/%d. +%d monedas, +%d velocidad.",
			index,
			Config.CheckpointCount,
			reward,
			Config.CheckpointSpeedBonus
		))
		FX:FireClient(player, "Checkpoint", index)
		checkMissions(player, data, "Checkpoint")
		SyncState:FireClient(player, snapshot(data))
	end)
end

for index, core in ipairs(heistCores) do
	core.Touched:Connect(function(hit)
		local player = Players:GetPlayerFromCharacter(hit.Parent)
		if not player then
			return
		end
		local data = DataService.Get(player)
		if not data or index ~= (data.HeistCores or 0) + 1 then
			return
		end
		data.HeistCores = index
		local reward = Config.HeistCoreReward * index
		data.Coins += reward
		notify(player, string.format("NUCLEO %d/%d asegurado. +%d monedas. Extrae en la meta.", index, Config.HeistCoreCount, reward))
		FX:FireClient(player, "HeistCore", heistCores[index].Color)
		SyncState:FireClient(player, snapshot(data))
	end)
end

local function collectCoin(player, data, entry)
	local part = entry.Part
	if not part or not part.Parent or not part.CanTouch then
		return
	end
	part.CanTouch = false
	part.Transparency = 1
	local multiplier = coinMultiplier(player, data) * Config.GetComboMultiplier(currentCombo(player))
	local amount = math.max(1, math.floor(entry.Value * multiplier))
	data.Coins += amount
	data.Missions.Coins.Progress += amount
	FX:FireClient(player, "Moneda")
	checkMissions(player, data, "Coins")
	SyncState:FireClient(player, snapshot(data))
	task.delay(Config.CoinRespawn, function()
		if part.Parent then
			part.Transparency = 0
			part.CanTouch = true
		end
	end)
end

for _, entry in ipairs(coins) do
	entry.Part.Touched:Connect(function(hit)
		local player = Players:GetPlayerFromCharacter(hit.Parent)
		if not player then
			return
		end
		local data = DataService.Get(player)
		if not data then
			return
		end
		collectCoin(player, data, entry)
	end)
end

-- Iman: recoge monedas cercanas automaticamente.
task.spawn(function()
	while true do
		task.wait(0.25)
		for _, player in ipairs(Players:GetPlayers()) do
			local data = DataService.Get(player)
			local level = data and data.Upgrades.Magnet or 0
			if level > 0 then
				local character = player.Character
				local root = character and character:FindFirstChild("HumanoidRootPart")
				if root then
					local radius = Config.MagnetBase + level * Config.Upgrades.Magnet.Effect
					for _, entry in ipairs(coins) do
						if entry.Part.CanTouch and (root.Position - entry.Part.Position).Magnitude <= radius then
							collectCoin(player, data, entry)
						end
					end
				end
			end
		end
	end
end)

for _, pad in ipairs(mapData.boostPads) do
	pad.Touched:Connect(function(hit)
		local player = Players:GetPlayerFromCharacter(hit.Parent)
		if not player then
			return
		end
		local data = DataService.Get(player)
		if not data then
			return
		end
		local now = os.clock()
		if (boostCooldowns[player] or 0) > now then
			return
		end
		boostCooldowns[player] = now + Config.BoostCooldown
		data.Speed += Config.BoostAmount
		updateBest(data)
		notify(player, string.format("BOOST. +%d velocidad.", Config.BoostAmount))
		FX:FireClient(player, "Boost")
	end)
end

local function connectHazard(part)
	part.Touched:Connect(function(hit)
		local player = Players:GetPlayerFromCharacter(hit.Parent)
		if not player then
			return
		end
		local data = DataService.Get(player)
		if not data then
			return
		end
		local now = os.clock()
		if (obstacleCooldowns[player] or 0) > now then
			return
		end
		obstacleCooldowns[player] = now + Config.ObstacleCooldown
		local lost = math.floor(data.Speed * Config.ObstaclePenalty)
		if lost <= 0 then
			return
		end
		data.Speed = math.max(0, data.Speed - lost)
		notify(player, string.format("CHOQUE. -%d velocidad.", lost))
		FX:FireClient(player, "Choque")
	end)
end

for _, obstacle in ipairs(mapData.obstacles) do
	connectHazard(obstacle)
end
for _, sweeper in ipairs(mapData.sweepers) do
	connectHazard(sweeper.Part)
end

mapData.finishTrigger.Touched:Connect(function(hit)
	local player = Players:GetPlayerFromCharacter(hit.Parent)
	if not player then
		return
	end
	local data = DataService.Get(player)
	if not data then
		return
	end
	if (data.HeistCores or 0) < Config.HeistCoreCount then
		notify(player, string.format("Te faltan %d nucleos para extraer.", Config.HeistCoreCount - data.HeistCores))
		return
	end
	local now = os.clock()
	if (finishCooldowns[player] or 0) > now then
		return
	end
	finishCooldowns[player] = now + 3
	local reward = math.floor(
		(Config.FinishReward + Config.HeistCompletionReward)
			* coinMultiplier(player, data)
			* Config.GetComboMultiplier(currentCombo(player))
	)
	data.Coins += reward
	data.Checkpoint = 0
	data.HeistCores = 0
	local combo = comboState[player]
	if combo then
		combo.Count = 0
	end
	teleportToCheckpoint(player)
	notify(player, string.format("META. +%d monedas. Vuelve a correr.", reward))
	FX:FireClient(player, "Meta")
	SyncState:FireClient(player, snapshot(data))
end)

mapData.voidPlane.Touched:Connect(function(hit)
	local player = Players:GetPlayerFromCharacter(hit.Parent)
	if not player then
		return
	end
	teleportToCheckpoint(player)
end)

-- Ranking de la partida (los mejores records de los jugadores conectados).
local function sessionLeaderboard()
	local list = {}
	for _, player in ipairs(Players:GetPlayers()) do
		local data = DataService.Get(player)
		if data then
			table.insert(list, {
				Name = player.Name,
				DisplayName = player.DisplayName,
				Value = math.floor(data.Best or 0),
			})
		end
	end
	table.sort(list, function(a, b)
		if a.Value == b.Value then
			return a.Name < b.Name
		end
		return a.Value > b.Value
	end)
	local top = {}
	for index = 1, math.min(#list, 10) do
		top[index] = list[index]
	end
	return top
end

task.spawn(function()
	while true do
		Leaderboard:FireAllClients(sessionLeaderboard())
		task.wait(5)
	end
end)

-- Auto-velocidad pasiva.
task.spawn(function()
	while true do
		task.wait(1)
		for _, player in ipairs(Players:GetPlayers()) do
			local data = DataService.Get(player)
			if data then
				local level = data.Upgrades.Auto or 0
				if level > 0 then
					data.Speed += level * Config.Upgrades.Auto.Effect
					updateBest(data)
				end
			end
		end
	end
end)

-- Sincronizacion de stats, velocidad fisica y combo.
task.spawn(function()
	local syncTick = 0
	while true do
		task.wait(0.1)
		syncTick += 1
		local now = os.clock()
		for _, player in ipairs(Players:GetPlayers()) do
			local data = DataService.Get(player)
			if data then
				updateBest(data)
				local combo = comboState[player]
				if combo and combo.Count > 0 and now - combo.Last > Config.ComboWindow then
					combo.Count = 0
				end
				local stats = player:FindFirstChild("leaderstats")
				if stats then
					stats.Speed.Value = math.floor(data.Speed)
					stats.Coins.Value = math.floor(data.Coins)
					stats.Rebirths.Value = data.Rebirths
					stats.Best.Value = math.floor(data.Best or 0)
				end
				applyHumanoid(player, data)
				if syncTick % 20 == 0 then
					SyncState:FireClient(player, snapshot(data))
				end
			end
		end
	end
end)

task.spawn(function()
	while true do
		task.wait(60)
		for _, player in ipairs(Players:GetPlayers()) do
			task.spawn(DataService.Save, player)
		end
	end
end)

Players.PlayerRemoving:Connect(function(player)
	DataService.Save(player)
	DataService.Release(player)
	passCache[player] = nil
	keyBuckets[player] = nil
	comboState[player] = nil
	boostCooldowns[player] = nil
	obstacleCooldowns[player] = nil
	finishCooldowns[player] = nil
end)

game:BindToClose(function()
	for _, player in ipairs(Players:GetPlayers()) do
		DataService.Save(player)
	end
	task.wait(1)
end)
