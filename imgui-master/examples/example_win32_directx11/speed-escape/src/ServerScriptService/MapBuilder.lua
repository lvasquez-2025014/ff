-- Construye la pista completa por codigo al iniciar el servidor.
local Workspace = game:GetService("Workspace")
local Lighting = game:GetService("Lighting")
local ReplicatedStorage = game:GetService("ReplicatedStorage")
local Config = require(ReplicatedStorage:WaitForChild("Shared"):WaitForChild("Config"))

local MapBuilder = {}

local DANGER_COLOR = Color3.fromRGB(255, 70, 80)
local COIN_COLOR = Color3.fromRGB(255, 210, 50)
local BOOST_COLOR = Color3.fromRGB(255, 220, 70)
local FINISH_COLOR = Color3.fromRGB(80, 220, 255)

local function makePart(props)
	local part = Instance.new("Part")
	part.Anchored = true
	part.TopSurface = Enum.SurfaceType.Smooth
	part.BottomSurface = Enum.SurfaceType.Smooth
	part.Material = Enum.Material.SmoothPlastic
	part.CanCollide = true
	for key, value in pairs(props) do
		if key ~= "Parent" then
			part[key] = value
		end
	end
	part.Parent = props.Parent
	return part
end

local function ensureEffect(className, name)
	local effect = Lighting:FindFirstChild(name)
	if not effect then
		effect = Instance.new(className)
		effect.Name = name
		effect.Parent = Lighting
	end
	return effect
end

local function setLightingProperty(property, value)
	pcall(function()
		Lighting[property] = value
	end)
end

local function clearOld()
	local old = Workspace:FindFirstChild("SpeedEscapeMap")
	if old then
		old:Destroy()
	end
	local oldSpawn = Workspace:FindFirstChild("SpeedEscapeSpawn")
	if oldSpawn then
		oldSpawn:Destroy()
	end
	local baseplate = Workspace:FindFirstChild("Baseplate")
	if baseplate and baseplate:IsA("BasePart") then
		baseplate:Destroy()
	end
	local defaultSpawn = Workspace:FindFirstChild("SpawnLocation")
	if defaultSpawn and defaultSpawn:IsA("SpawnLocation") then
		defaultSpawn:Destroy()
	end
end

local function addBillboard(adornee, text, color, offsetY, width, height)
	local billboard = Instance.new("BillboardGui")
	billboard.Name = "Label"
	billboard.Adornee = adornee
	billboard.Size = UDim2.fromOffset(width or 260, height or 54)
	billboard.StudsOffset = Vector3.new(0, offsetY or 7, 0)
	billboard.AlwaysOnTop = true
	billboard.LightInfluence = 0
	billboard.MaxDistance = 900
	billboard.Parent = adornee

	local label = Instance.new("TextLabel")
	label.Size = UDim2.fromScale(1, 1)
	label.BackgroundTransparency = 1
	label.Text = text
	label.TextColor3 = color
	label.TextStrokeTransparency = 0.3
	label.TextStrokeColor3 = Color3.new(0, 0, 0)
	label.Font = Enum.Font.GothamBlack
	label.TextScaled = true
	label.Parent = billboard
end

local function addPointLight(parent, color, brightness, range)
	local light = Instance.new("PointLight")
	light.Color = color
	light.Brightness = brightness or 1
	light.Range = range or 18
	light.Shadows = false
	light.Parent = parent
	return light
end

local function zoneAtZ(z)
	local index = math.clamp(math.floor((-z) / Config.ZoneLength) + 1, 1, #Config.Zones)
	return Config.Zones[index], index
end

local function isClear(z, list, minDistance)
	for _, other in ipairs(list) do
		if math.abs(z - other) < minDistance then
			return false
		end
	end
	return true
end

function MapBuilder.Build()
	clearOld()

	local map = Instance.new("Folder")
	map.Name = "SpeedEscapeMap"

	local track = Instance.new("Folder")
	track.Name = "Track"
	track.Parent = map

	local hazards = Instance.new("Folder")
	hazards.Name = "Hazards"
	hazards.Parent = map

	local checkpointsFolder = Instance.new("Folder")
	checkpointsFolder.Name = "Checkpoints"
	checkpointsFolder.Parent = map

	local effects = Instance.new("Folder")
	effects.Name = "Effects"
	effects.Parent = map

	local coinsFolder = Instance.new("Folder")
	coinsFolder.Name = "Coins"
	coinsFolder.Parent = map

	local decor = Instance.new("Folder")
	decor.Name = "Decor"
	decor.Parent = map

	-- Spawn directo en Workspace (los SpawnLocation dentro de carpetas no siempre funcionan).
	local spawn = Instance.new("SpawnLocation")
	spawn.Name = "SpeedEscapeSpawn"
	spawn.Anchored = true
	spawn.Size = Vector3.new(34, 1, 40)
	spawn.CFrame = CFrame.new(0, 0.5, 20)
	spawn.Neutral = true
	spawn.Duration = 0
	spawn.Material = Enum.Material.SmoothPlastic
	spawn.Color = Color3.fromRGB(40, 52, 90)
	spawn.TopSurface = Enum.SurfaceType.Smooth
	spawn.Parent = Workspace
	makePart({
		Name = "LobbyFloor",
		Size = Vector3.new(62, 0.28, 72),
		CFrame = CFrame.new(0, 0.92, 36),
		Color = Color3.fromRGB(14, 18, 38),
		Material = Enum.Material.Metal,
		CanCollide = false,
		Parent = decor,
	})
	for _, edge in ipairs({
		{ Vector3.new(62, 0.22, 0.8), Vector3.new(0, 1.12, 1) },
		{ Vector3.new(62, 0.22, 0.8), Vector3.new(0, 1.12, 71) },
		{ Vector3.new(0.8, 0.22, 70), Vector3.new(-31, 1.12, 36) },
		{ Vector3.new(0.8, 0.22, 70), Vector3.new(31, 1.12, 36) },
	}) do
		makePart({
			Name = "LobbyEdge",
			Size = edge[1],
			CFrame = CFrame.new(edge[2]),
			Color = Color3.fromRGB(176, 112, 255),
			Material = Enum.Material.Neon,
			CanCollide = false,
			Parent = decor,
		})
	end
	for _, x in ipairs({ -26, 26 }) do
		makePart({
			Name = "LobbyPillar",
			Size = Vector3.new(2.5, 14, 2.5),
			CFrame = CFrame.new(x, 7, 54),
			Color = Color3.fromRGB(80, 220, 255),
			Material = Enum.Material.Neon,
			CanCollide = false,
			Parent = decor,
		})
	end
	local lobbyTop = makePart({
		Name = "LobbyTop",
		Size = Vector3.new(54, 2, 2),
		CFrame = CFrame.new(0, 14, 54),
		Color = Color3.fromRGB(80, 220, 255),
		Material = Enum.Material.Neon,
		CanCollide = false,
		Parent = decor,
	})
	addBillboard(lobbyTop, "START RUN", Color3.fromRGB(240, 250, 255), 4, 300, 58)
	local lobbyWall = makePart({
		Name = "LobbyBackWall",
		Size = Vector3.new(58, 15, 0.8),
		CFrame = CFrame.new(0, 8, 66),
		Color = Color3.fromRGB(9, 13, 30),
		Material = Enum.Material.Metal,
		CanCollide = false,
		Parent = decor,
	})
	addBillboard(lobbyWall, "OPERACION // HEIST CONTROL", Color3.fromRGB(220, 235, 255), 7, 520, 54)
	for panelIndex, panelData in ipairs({
		{ -18, Color3.fromRGB(80, 220, 255), "01  //  NUCLEO AZUL" },
		{ 0, Color3.fromRGB(255, 90, 220), "02  //  NUCLEO MAGENTA" },
		{ 18, Color3.fromRGB(255, 215, 60), "03  //  EXTRACCION" },
	}) do
		local panel = makePart({
			Name = "LobbyMissionPanel_" .. panelIndex,
			Size = Vector3.new(14, 7, 0.3),
			CFrame = CFrame.new(panelData[1], 8, 65.45),
			Color = Color3.fromRGB(18, 25, 48),
			Material = Enum.Material.Metal,
			CanCollide = false,
			Parent = decor,
		})
		local panelEdge = makePart({
			Name = "LobbyMissionGlow_" .. panelIndex,
			Size = Vector3.new(12.5, 0.18, 0.38),
			CFrame = CFrame.new(panelData[1], 11.3, 65.15),
			Color = panelData[2],
			Material = Enum.Material.Neon,
			CanCollide = false,
			Parent = decor,
		})
		addPointLight(panelEdge, panelData[2], 0.7, 16)
		addBillboard(panel, panelData[3], panelData[2], 0.3, 230, 42)
	end
	local lobbyHologram = makePart({
		Name = "LobbyHologram",
		Size = Vector3.new(5.5, 5.5, 5.5),
		CFrame = CFrame.new(0, 8.2, 51),
		Shape = Enum.PartType.Ball,
		Color = Color3.fromRGB(80, 220, 255),
		Material = Enum.Material.ForceField,
		Transparency = 0.18,
		CanCollide = false,
		Parent = decor,
	})
	addPointLight(lobbyHologram, Color3.fromRGB(80, 220, 255), 1.2, 22)
	addBillboard(lobbyHologram, "RUN 01  //  ONLINE", Color3.fromRGB(220, 245, 255), 4.2, 260, 40)
	local hologramRings = {}
	for ringIndex, radius in ipairs({ 4.6, 6.3, 8 }) do
		for segment = 1, 12 do
			local angle = ((segment - 1) / 12) * math.pi * 2
			local ringPart = makePart({
				Name = "LobbyHologramRing",
				Size = Vector3.new(2.8, 0.16, 0.35),
				CFrame = CFrame.new(math.cos(angle) * radius, 8.2 + (ringIndex - 2) * 0.8, 51 + math.sin(angle) * radius)
					* CFrame.Angles(0, -angle, ringIndex == 2 and math.rad(18) or 0),
				Color = ringIndex == 3 and Color3.fromRGB(176, 112, 255) or Color3.fromRGB(80, 220, 255),
				Material = Enum.Material.Neon,
				Transparency = 0.22,
				CanCollide = false,
				Parent = decor,
			})
			table.insert(hologramRings, ringPart)
		end
	end
	local hologramParticles = Instance.new("ParticleEmitter")
	hologramParticles.Rate = 7
	hologramParticles.Lifetime = NumberRange.new(0.8, 1.5)
	hologramParticles.Speed = NumberRange.new(0.15, 0.5)
	hologramParticles.SpreadAngle = Vector2.new(360, 360)
	hologramParticles.Size = NumberSequence.new({
		NumberSequenceKeypoint.new(0, 0.16),
		NumberSequenceKeypoint.new(1, 0),
	})
	hologramParticles.LightEmission = 1
	hologramParticles.Color = ColorSequence.new(Color3.fromRGB(80, 220, 255))
	hologramParticles.Parent = lobbyHologram
	task.spawn(function()
		local elapsed = 0
		while lobbyHologram.Parent do
			elapsed += task.wait(1 / 30)
			lobbyHologram.CFrame = CFrame.new(0, 8.2 + math.sin(elapsed * 1.5) * 0.35, 51)
			for ringIndex, ringPart in ipairs(hologramRings) do
				if ringPart.Parent then
					ringPart.CFrame *= CFrame.Angles(0, 0.01 * (ringIndex % 3 + 1), 0)
				end
			end
		end
	end)
	makePart({
		Name = "SpawnGlow",
		Size = Vector3.new(36, 0.18, 42),
		CFrame = CFrame.new(0, 1.05, 20),
		Color = Color3.fromRGB(80, 220, 255),
		Material = Enum.Material.Neon,
		Transparency = 0.65,
		CanCollide = false,
		Parent = decor,
	})
	for _, x in ipairs({ -16, 16 }) do
		makePart({
			Name = "SpawnLight",
			Size = Vector3.new(0.7, 5, 0.7),
			CFrame = CFrame.new(x, 3, 20),
			Color = Color3.fromRGB(176, 112, 255),
			Material = Enum.Material.Neon,
			CanCollide = false,
			Parent = decor,
		})
	end
	addBillboard(spawn, "RUN 01  // 3 CORES -> EXIT", Color3.fromRGB(255, 215, 60), 5, 330, 48)

	-- Suelo por zonas.
	local segmentLength = 20
	local segmentCount = math.ceil(Config.TrackLength / segmentLength)
	for i = 0, segmentCount - 1 do
		local z = -(i * segmentLength) - segmentLength / 2
		local zone = zoneAtZ(z)
		makePart({
			Name = "Floor_" .. i,
			Size = Vector3.new(Config.TrackWidth, 2, segmentLength),
			CFrame = CFrame.new(0, 0, z),
			Color = (i % 2 == 0) and zone.FloorA or zone.FloorB,
			Material = Enum.Material.Asphalt,
			Parent = track,
		})
	end
	-- Base exterior: evita la sensacion de pista flotando y da profundidad al skyline.
	makePart({
		Name = "CityUnderlay",
		Size = Vector3.new(150, 1.2, Config.TrackLength + 320),
		CFrame = CFrame.new(0, -1.35, -Config.TrackLength / 2),
		Color = Color3.fromRGB(6, 9, 22),
		Material = Enum.Material.Slate,
		CanCollide = false,
		Parent = track,
	})
	for zoneIndex = 1, #Config.Zones do
		local zone = Config.Zones[zoneIndex]
		local zoneStart = -((zoneIndex - 1) * Config.ZoneLength)
		for _, x in ipairs({ -57, 57 }) do
			makePart({
				Name = "CityBank",
				Size = Vector3.new(34, 0.16, Config.ZoneLength - 6),
				CFrame = CFrame.new(x, -0.68, zoneStart - Config.ZoneLength / 2),
				Color = zone.FloorA,
				Material = Enum.Material.Metal,
				Transparency = 0.18,
				CanCollide = false,
				Parent = decor,
			})
			makePart({
				Name = "CityBankEdge",
				Size = Vector3.new(0.7, 0.28, Config.ZoneLength - 8),
				CFrame = CFrame.new(x + (x < 0 and 17 or -17), -0.52, zoneStart - Config.ZoneLength / 2),
				Color = zone.Accent,
				Material = Enum.Material.Neon,
				Transparency = 0.35,
				CanCollide = false,
				Parent = decor,
			})
		end
	end

	-- Rieles neon en los bordes de cada zona.
	for zoneIndex = 1, #Config.Zones do
		local zone = Config.Zones[zoneIndex]
		local centerZ = -((zoneIndex - 1) * Config.ZoneLength + Config.ZoneLength / 2)
		local zoneStart = -((zoneIndex - 1) * Config.ZoneLength)
		for _, x in ipairs({ -Config.TrackWidth / 2 - 0.8, Config.TrackWidth / 2 + 0.8 }) do
			makePart({
				Name = "Rail",
				Size = Vector3.new(1.2, 0.5, Config.ZoneLength),
				CFrame = CFrame.new(x, 1.4, centerZ),
				Color = zone.Accent,
				Material = Enum.Material.Neon,
				CanCollide = false,
				Parent = decor,
			})
		end
		-- Separadores de carril: ayudan a leer la velocidad y dan ritmo visual.
		for _, x in ipairs({ -11, 0, 11 }) do
			makePart({
				Name = "LaneGuide",
				Size = Vector3.new(0.18, 0.08, Config.ZoneLength),
				CFrame = CFrame.new(x, 1.08, centerZ),
				Color = zone.Accent,
				Material = Enum.Material.Neon,
				Transparency = 0.72,
				CanCollide = false,
				Parent = decor,
			})
		end
		-- Franjas cortas en la calzada: dan escala y una sensacion clara de avance.
		for dash = 1, 13 do
			makePart({
				Name = "RoadDash",
				Size = Vector3.new(Config.TrackWidth - 5, 0.06, 0.7),
				CFrame = CFrame.new(0, 1.1, zoneStart - dash * 72),
				Color = zone.Accent,
				Material = Enum.Material.Neon,
				Transparency = 0.86,
				CanCollide = false,
				Parent = decor,
			})
		end
		-- Marcadores de velocidad en el suelo: crean una lectura continua de avance.
		for marker = 1, 5 do
			local markerZ = zoneStart - marker * 165
			for _, x in ipairs({ -16.5, -5.5, 5.5, 16.5 }) do
				makePart({
					Name = "SpeedMarker",
					Size = Vector3.new(5.5, 0.1, 1.2),
					CFrame = CFrame.new(x, 1.08, markerZ),
					Color = zone.Accent,
					Material = Enum.Material.Neon,
					Transparency = 0.38,
					CanCollide = false,
					Parent = decor,
				})
			end
		end
	end

	-- Skyline lateral para evitar tramos vacios y reforzar la identidad de cada zona.
	for i = 0, math.floor(Config.TrackLength / 180) do
		local z = -(i * 180) - 90
		local zone = zoneAtZ(z)
		local height = 7 + ((i * 17) % 15)
		for sideIndex, x in ipairs({ -34, 34 }) do
			local building = makePart({
				Name = "NeonTower",
				Size = Vector3.new(7, height, 12),
				CFrame = CFrame.new(x, height / 2, z + (sideIndex == 2 and 22 or -22)),
				Color = zone.FloorA:Lerp(Color3.fromRGB(5, 7, 18), 0.45),
				Material = Enum.Material.Metal,
				CanCollide = false,
				Parent = decor,
			})
			makePart({
				Name = "TowerGlow",
				Size = Vector3.new(0.35, math.max(3, height - 2), 0.35),
				CFrame = building.CFrame * CFrame.new(sideIndex == 1 and 3.4 or -3.4, 0, -6.1),
				Color = zone.Accent,
				Material = Enum.Material.Neon,
				CanCollide = false,
				Parent = decor,
			})
			for window = 1, 2 do
				makePart({
					Name = "TowerWindow",
					Size = Vector3.new(0.28, 0.28, 4.6),
					CFrame = building.CFrame * CFrame.new(sideIndex == 1 and 3.55 or -3.55, -height / 2 + window * height / 3, 0),
					Color = zone.Accent,
					Material = Enum.Material.Neon,
					Transparency = 0.35,
					CanCollide = false,
					Parent = decor,
				})
			end
		end
	end

	-- Titulo en la salida.
	local startLeft = makePart({
		Name = "StartPillar",
		Size = Vector3.new(3, 18, 3),
		CFrame = CFrame.new(-20, 9, 12),
		Color = Color3.fromRGB(80, 220, 255),
		Material = Enum.Material.Neon,
		CanCollide = false,
		Parent = decor,
	})
	makePart({
		Name = "StartPillar",
		Size = Vector3.new(3, 18, 3),
		CFrame = CFrame.new(20, 9, 12),
		Color = Color3.fromRGB(80, 220, 255),
		Material = Enum.Material.Neon,
		CanCollide = false,
		Parent = decor,
	})
	local startTop = makePart({
		Name = "StartTop",
		Size = Vector3.new(43, 3, 3),
		CFrame = CFrame.new(0, 18, 12),
		Color = Color3.fromRGB(80, 220, 255),
		Material = Enum.Material.Neon,
		CanCollide = false,
		Parent = decor,
	})
	addBillboard(startTop, "NEON HEIST", Color3.fromRGB(240, 250, 255), 4, 420, 70)
	addBillboard(startLeft, "ASEGURA 3 NUCLEOS", Color3.fromRGB(150, 230, 255), 4, 260, 40)

	local checkpoints = {}
	local reserved = {}

	-- Checkpoints.
	for i = 1, Config.CheckpointCount do
		local z = -i * Config.CheckpointSpacing
		local zone = zoneAtZ(z)
		local strip = makePart({
			Name = "Checkpoint_" .. i,
			Size = Vector3.new(Config.TrackWidth, 0.4, 8),
			CFrame = CFrame.new(0, 1.2, z),
			Color = zone.Accent,
			Material = Enum.Material.Neon,
			CanCollide = false,
			Parent = checkpointsFolder,
		})
		strip:SetAttribute("CheckpointIndex", i)
		addBillboard(strip, "CHECKPOINT " .. i, zone.Accent, 7, 240, 48)
		for _, x in ipairs({ -22, 22 }) do
			makePart({
				Name = "CheckpointPillar_" .. i,
				Size = Vector3.new(1.5, 10, 1.5),
				CFrame = CFrame.new(x, 6, z),
				Color = zone.Accent,
				Material = Enum.Material.Neon,
				CanCollide = false,
				Parent = decor,
			})
		end
		makePart({
			Name = "CheckpointBeam_" .. i,
			Size = Vector3.new(46, 1.5, 1.5),
			CFrame = CFrame.new(0, 11, z),
			Color = zone.Accent,
			Material = Enum.Material.Neon,
			CanCollide = false,
			Parent = decor,
		})
		checkpoints[i] = strip
		table.insert(reserved, z)
	end

	local transitRails = {}
	-- Arcos de zona.
	for zoneIndex = 2, #Config.Zones do
		local zone = Config.Zones[zoneIndex]
		local z = -((zoneIndex - 1) * Config.ZoneLength)
		for _, x in ipairs({ -23.5, 23.5 }) do
			makePart({
				Name = "ZonePillar",
				Size = Vector3.new(2.5, 16, 2.5),
				CFrame = CFrame.new(x, 8, z),
				Color = zone.Accent,
				Material = Enum.Material.Neon,
				CanCollide = false,
				Parent = decor,
			})
		end
		local top = makePart({
			Name = "ZoneTop",
			Size = Vector3.new(50, 2.5, 2.5),
			CFrame = CFrame.new(0, 16, z),
			Color = zone.Accent,
			Material = Enum.Material.Neon,
			CanCollide = false,
			Parent = decor,
		})
		addBillboard(top, zoneIndex .. ". " .. zone.Name, zone.Accent, 4, 460, 60)
		-- Portal circular suspendido: cada zona tiene una silueta propia al entrar.
		local ringRadius = 16
		local ringSegments = 14
		for segment = 1, ringSegments do
			local angle = ((segment - 1) / ringSegments) * math.pi * 2
			local nextAngle = (segment / ringSegments) * math.pi * 2
			local a = Vector3.new(math.cos(angle) * ringRadius, 18 + math.sin(angle) * ringRadius, z)
			local b = Vector3.new(math.cos(nextAngle) * ringRadius, 18 + math.sin(nextAngle) * ringRadius, z)
			local midpoint = (a + b) / 2
			local length = (b - a).Magnitude
			local ringBeam = makePart({
				Name = "ZoneRing",
				Size = Vector3.new(length, 0.7, 0.7),
				CFrame = CFrame.lookAt(midpoint, b) * CFrame.Angles(0, math.pi / 2, 0),
				Color = zone.Accent,
				Material = Enum.Material.Neon,
				CanCollide = false,
				Parent = decor,
			})
			if segment % 4 == 0 then
				addPointLight(ringBeam, zone.Accent, 0.35, 12)
			end
		end
	end

	-- Pasos elevados: rompen la lectura plana y hacen que cada frontera tenga escala.
	for zoneIndex = 2, #Config.Zones do
		local zone = Config.Zones[zoneIndex]
		local bridgeZ = -((zoneIndex - 1) * Config.ZoneLength) - 120
		local bridge = makePart({
			Name = "TransitBridge_" .. zoneIndex,
			Size = Vector3.new(66, 1.1, 16),
			CFrame = CFrame.new(0, 23, bridgeZ),
			Color = Color3.fromRGB(13, 18, 38),
			Material = Enum.Material.Metal,
			CanCollide = false,
			Parent = decor,
		})
		addBillboard(bridge, "TRANSIT // SECTOR " .. zoneIndex, zone.Accent, 1.5, 300, 40)
		for _, x in ipairs({ -30, 30 }) do
			makePart({
				Name = "TransitBridgePillar",
				Size = Vector3.new(1.4, 22, 1.4),
				CFrame = CFrame.new(x, 11.5, bridgeZ),
				Color = Color3.fromRGB(28, 32, 58),
				Material = Enum.Material.Metal,
				CanCollide = false,
				Parent = decor,
			})
		end
		for _, zOffset in ipairs({ -7.2, 7.2 }) do
			local bridgeRail = makePart({
				Name = "TransitBridgeRail",
				Size = Vector3.new(63, 0.35, 0.5),
				CFrame = CFrame.new(0, 23.8, bridgeZ + zOffset),
				Color = zone.Accent,
				Material = Enum.Material.Neon,
				CanCollide = false,
				Parent = decor,
			})
			table.insert(transitRails, bridgeRail)
			if zOffset < 0 then
				addPointLight(bridgeRail, zone.Accent, 0.4, 16)
			end
		end
	end

	-- Iluminacion lateral repetible: crea profundidad sin llenar la pista de geometria pesada.
	for i = 0, math.floor(Config.TrackLength / 240) do
		local poleZ = -(i * 240) - 120
		local zone, zoneIndex = zoneAtZ(poleZ)
		for sideIndex, x in ipairs({ -28, 28 }) do
			local pole = makePart({
				Name = "StreetLightPole",
				Size = Vector3.new(0.65, 11, 0.65),
				CFrame = CFrame.new(x, 5.5, poleZ),
				Color = Color3.fromRGB(19, 24, 46),
				Material = Enum.Material.Metal,
				CanCollide = false,
				Parent = decor,
			})
			makePart({
				Name = "StreetLightArm",
				Size = Vector3.new(5.5, 0.22, 0.22),
				CFrame = CFrame.new(x + (sideIndex == 1 and 2.5 or -2.5), 10.7, poleZ),
				Color = zone.Accent,
				Material = Enum.Material.Neon,
				CanCollide = false,
				Parent = decor,
			})
			local lamp = makePart({
				Name = "StreetLight",
				Size = Vector3.new(0.9, 0.25, 0.9),
				CFrame = CFrame.new(x + (sideIndex == 1 and 5 or -5), 10.55, poleZ),
				Color = zone.Accent,
				Material = Enum.Material.Neon,
				CanCollide = false,
				Parent = decor,
			})
			addPointLight(lamp, zone.Accent, 0.8, 20)
			pole:SetAttribute("ZoneIndex", zoneIndex)
		end
	end

	-- Set pieces: cada sector cambia su silueta para que la carrera tenga momentos memorables.
	-- Sector 2: tunel magenta con costillas luminosas.
	for z = -1120, -1880, -190 do
		for _, x in ipairs({ -24, 24 }) do
			makePart({
				Name = "MagentaTunnelWall",
				Size = Vector3.new(2.2, 15, 2.2),
				CFrame = CFrame.new(x, 7.5, z),
				Color = Color3.fromRGB(42, 12, 55),
				Material = Enum.Material.Metal,
				CanCollide = false,
				Parent = decor,
			})
		end
		local rib = makePart({
			Name = "MagentaTunnelRib",
			Size = Vector3.new(50, 0.9, 1.1),
			CFrame = CFrame.new(0, 15, z),
			Color = Color3.fromRGB(255, 90, 220),
			Material = Enum.Material.Neon,
			CanCollide = false,
			Parent = decor,
		})
		addPointLight(rib, Color3.fromRGB(255, 90, 220), 0.5, 18)
	end
	makePart({
		Name = "MagentaTunnelRoof",
		Size = Vector3.new(50, 1.2, 820),
		CFrame = CFrame.new(0, 16, -1500),
		Color = Color3.fromRGB(24, 10, 35),
		Material = Enum.Material.Metal,
		CanCollide = false,
		Parent = decor,
	})

	-- Sector 3: tanques toxicos laterales y conductos verdes.
	for tankIndex, z in ipairs({ -2120, -2470, -2820 }) do
		for sideIndex, x in ipairs({ -31, 31 }) do
			local tank = makePart({
				Name = "ToxicTank_" .. tankIndex,
				Size = Vector3.new(14, 7, 7),
				CFrame = CFrame.new(x, 7, z + (sideIndex == 2 and 45 or -45)) * CFrame.Angles(0, 0, math.rad(90)),
				Shape = Enum.PartType.Cylinder,
				Color = Color3.fromRGB(14, 48, 36),
				Material = Enum.Material.Metal,
				CanCollide = false,
				Parent = decor,
			})
			local tankGlow = makePart({
				Name = "ToxicTankGlow_" .. tankIndex,
				Size = Vector3.new(0.5, 9, 0.5),
				CFrame = tank.CFrame * CFrame.new(0, 0, 4.05),
				Color = Color3.fromRGB(80, 255, 140),
				Material = Enum.Material.Neon,
				CanCollide = false,
				Parent = decor,
			})
			addPointLight(tankGlow, Color3.fromRGB(80, 255, 140), 0.45, 14)
		end
	end

	-- Sector 4: cuchillas solares verticales que se leen como una planta energetica.
	for bladeIndex = 1, 7 do
		local bladeZ = -3150 - bladeIndex * 110
		for sideIndex, x in ipairs({ -29, 29 }) do
			local blade = makePart({
				Name = "SolarBlade",
				Size = Vector3.new(1.1, 24, 5),
				CFrame = CFrame.new(x, 12, bladeZ) * CFrame.Angles(0, math.rad(sideIndex == 1 and -22 or 22), 0),
				Color = Color3.fromRGB(255, 170, 60),
				Material = Enum.Material.Neon,
				Transparency = 0.12,
				CanCollide = false,
				Parent = decor,
			})
			addPointLight(blade, Color3.fromRGB(255, 170, 60), 0.35, 14)
		end
	end

	-- Sector 5: columnas flotantes del abismo, con alturas desiguales.
	for pillarIndex = 1, 9 do
		local pillarZ = -4100 - pillarIndex * 90
		local height = 12 + ((pillarIndex * 7) % 18)
		for sideIndex, x in ipairs({ -32, 32 }) do
			makePart({
				Name = "AbyssPillar",
				Size = Vector3.new(3, height, 3),
				CFrame = CFrame.new(x, height / 2, pillarZ + (sideIndex == 2 and 30 or -30)),
				Color = Color3.fromRGB(38, 20, 70),
				Material = Enum.Material.Metal,
				CanCollide = false,
				Parent = decor,
			})
			makePart({
				Name = "AbyssPillarGlow",
				Size = Vector3.new(0.45, math.max(5, height - 2), 0.45),
				CFrame = CFrame.new(x + (sideIndex == 1 and 1.35 or -1.35), height / 2, pillarZ + (sideIndex == 2 and 30 or -30)),
				Color = Color3.fromRGB(170, 110, 255),
				Material = Enum.Material.Neon,
				CanCollide = false,
				Parent = decor,
			})
		end
	end

	local reactorCores = {}
	-- Sector 6: reactor final, un marco rojo que anticipa la extraccion.
	for reactorIndex, z in ipairs({ -5050, -5370, -5690 }) do
		local reactor = makePart({
			Name = "RedReactor",
			Size = Vector3.new(18, 9, 9),
			CFrame = CFrame.new(0, 9, z) * CFrame.Angles(0, 0, math.rad(90)),
			Shape = Enum.PartType.Cylinder,
			Color = Color3.fromRGB(58, 13, 24),
			Material = Enum.Material.Metal,
			CanCollide = false,
			Parent = decor,
		})
		local reactorCore = makePart({
			Name = "RedReactorCore",
			Size = Vector3.new(1.2, 12, 1.2),
			CFrame = reactor.CFrame * CFrame.new(0, 0, 4.6),
			Color = Color3.fromRGB(255, 80, 90),
			Material = Enum.Material.Neon,
			CanCollide = false,
			Parent = decor,
		})
		table.insert(reactorCores, reactorCore)
		addPointLight(reactorCore, Color3.fromRGB(255, 80, 90), 0.65, 18)
	end

	local lanes = { -16.5, -5.5, 5.5, 16.5 }
	local rng = Random.new(20261103)

	-- Plataformas de impulso.
	local boostPads = {}
	local boostZ = -300
	while boostZ > -Config.TrackLength + 100 do
		local lane = lanes[rng:NextInteger(1, #lanes)]
		local pad = makePart({
			Name = "BoostPad",
			Size = Vector3.new(10, 0.5, 10),
			CFrame = CFrame.new(lane, 1.2, boostZ),
			Color = BOOST_COLOR,
			Material = Enum.Material.Neon,
			CanCollide = false,
			Parent = effects,
		})
		pad:SetAttribute("BoostPad", true)
		for _, stripeX in ipairs({ -2.5, 0, 2.5 }) do
			makePart({
				Name = "BoostStripe",
				Size = Vector3.new(0.65, 0.12, 8.5),
				CFrame = CFrame.new(lane + stripeX, 1.5, boostZ),
				Color = Color3.fromRGB(255, 250, 180),
				Material = Enum.Material.Neon,
				CanCollide = false,
				Parent = effects,
			})
		end
		local boostHighlight = Instance.new("Highlight")
		boostHighlight.Adornee = pad
		boostHighlight.FillColor = BOOST_COLOR
		boostHighlight.FillTransparency = 0.78
		boostHighlight.OutlineColor = BOOST_COLOR
		boostHighlight.OutlineTransparency = 0.15
		boostHighlight.DepthMode = Enum.HighlightDepthMode.AlwaysOnTop
		boostHighlight.Parent = pad
		local boostLight = Instance.new("PointLight")
		boostLight.Color = BOOST_COLOR
		boostLight.Brightness = 2
		boostLight.Range = 14
		boostLight.Parent = pad
		local boostParticles = Instance.new("ParticleEmitter")
		boostParticles.Rate = 10
		boostParticles.Lifetime = NumberRange.new(0.25, 0.55)
		boostParticles.Speed = NumberRange.new(1, 3)
		boostParticles.SpreadAngle = Vector2.new(20, 20)
		boostParticles.Size = NumberSequence.new({
			NumberSequenceKeypoint.new(0, 0.18),
			NumberSequenceKeypoint.new(1, 0),
		})
		boostParticles.LightEmission = 1
		boostParticles.Color = ColorSequence.new(BOOST_COLOR)
		boostParticles.Parent = pad
		table.insert(boostPads, pad)
		table.insert(reserved, boostZ)
		boostZ -= 600
	end

	-- Obstaculos moviles: barras que giran y se saltan.
	local sweepers = {}
	for zoneIndex = 2, #Config.Zones do
		local z = -((zoneIndex - 1) * Config.ZoneLength) - 250
		local bar = makePart({
			Name = "Sweeper",
			Size = Vector3.new(Config.TrackWidth + 4, 1.4, 1.4),
			CFrame = CFrame.new(0, 3, z),
			Color = DANGER_COLOR,
			Material = Enum.Material.Neon,
			CanCollide = false,
			CanTouch = true,
			Parent = hazards,
		})
		bar:SetAttribute("Sweeper", true)
		makePart({
			Name = "SweeperCore",
			Size = Vector3.new(2.4, 2.4, 2.4),
			CFrame = CFrame.new(0, 3, z),
			Color = Color3.fromRGB(255, 160, 80),
			Material = Enum.Material.Neon,
			CanCollide = false,
			CanTouch = false,
			Parent = decor,
		})
		table.insert(sweepers, {
			Part = bar,
			Position = Vector3.new(0, 3, z),
			Speed = 0.85 + zoneIndex * 0.08,
			Angle = zoneIndex,
		})
		table.insert(reserved, z)
	end

	-- Obstaculos fijos.
	local obstacles = {}
	local z = -180
	while z > -Config.TrackLength + 200 do
		if isClear(z, reserved, 55) then
			local laneCount = rng:NextInteger(1, 2)
			local picked = {}
			local used = {}
			while #picked < laneCount do
				local lane = rng:NextInteger(1, #lanes)
				if not used[lane] then
					used[lane] = true
					table.insert(picked, lane)
				end
			end
			for _, lane in ipairs(picked) do
				local obstacle = makePart({
					Name = "Obstacle",
					Size = Vector3.new(9.5, 12, 6),
					CFrame = CFrame.new(lanes[lane], 7, z),
					Color = Color3.fromRGB(46, 18, 34),
					Material = Enum.Material.Metal,
					Parent = hazards,
				})
				for _, stripeX in ipairs({ -3.2, 0, 3.2 }) do
					makePart({
						Name = "HazardStripe",
						Size = Vector3.new(0.7, 9.5, 0.14),
						CFrame = CFrame.new(lanes[lane] + stripeX, 7, z - 3.08),
						Color = DANGER_COLOR,
						Material = Enum.Material.Neon,
						Transparency = 0.18,
						CanCollide = false,
						Parent = decor,
					})
				end
				local obstacleHighlight = Instance.new("Highlight")
				obstacleHighlight.Adornee = obstacle
				obstacleHighlight.FillColor = DANGER_COLOR
				obstacleHighlight.FillTransparency = 0.82
				obstacleHighlight.OutlineColor = DANGER_COLOR
				obstacleHighlight.OutlineTransparency = 0.08
				obstacleHighlight.DepthMode = Enum.HighlightDepthMode.AlwaysOnTop
				obstacleHighlight.Parent = obstacle
				table.insert(obstacles, obstacle)
			end
			table.insert(reserved, z)
		end
		z -= rng:NextNumber(120, 190)
	end

	-- Monedas coleccionables.
	local coins = {}
	for zoneIndex = 1, #Config.Zones do
		local zoneStart = -((zoneIndex - 1) * Config.ZoneLength)
		for _ = 1, Config.CoinPerZone do
			for _ = 1, 24 do
				local coinZ = zoneStart - rng:NextNumber(60, Config.ZoneLength - 60)
				if isClear(coinZ, reserved, 28) then
					local lane = lanes[rng:NextInteger(1, #lanes)]
					local position = Vector3.new(lane, 2.4, coinZ)
					local coin = Instance.new("Part")
					coin.Name = "Coin"
					coin.Shape = Enum.PartType.Cylinder
					coin.Size = Vector3.new(0.5, 4.5, 4.5)
					coin.CFrame = CFrame.new(position) * CFrame.Angles(0, math.rad(90), 0)
					coin.Anchored = true
					coin.CanCollide = false
					coin.CanTouch = true
					coin.Material = Enum.Material.Neon
					coin.Color = COIN_COLOR
					coin.Parent = coinsFolder
					table.insert(coins, {
						Part = coin,
						Value = Config.CoinValue + (zoneIndex - 1) * 5,
						Position = position,
					})
					table.insert(reserved, coinZ)
					break
				end
			end
		end
	end

	-- Meta.
	local finishZ = -Config.TrackLength
	makePart({
		Name = "ExtractionPlatform",
		Size = Vector3.new(60, 0.35, 42),
		CFrame = CFrame.new(0, 1.05, finishZ + 14),
		Color = Color3.fromRGB(22, 29, 58),
		Material = Enum.Material.Metal,
		CanCollide = false,
		Parent = effects,
	})
	for _, ringData in ipairs({
		{ 22, FINISH_COLOR },
		{ 16, Color3.fromRGB(176, 112, 255) },
		{ 10, Color3.fromRGB(255, 215, 60) },
	}) do
		local radius = ringData[1]
		local color = ringData[2]
		for _, segment in ipairs({
			{ Vector3.new(radius * 2, 0.22, 0.7), Vector3.new(0, 1.28, finishZ + 14 - radius) },
			{ Vector3.new(radius * 2, 0.22, 0.7), Vector3.new(0, 1.28, finishZ + 14 + radius) },
			{ Vector3.new(0.7, 0.22, radius * 2), Vector3.new(-radius, 1.28, finishZ + 14) },
			{ Vector3.new(0.7, 0.22, radius * 2), Vector3.new(radius, 1.28, finishZ + 14) },
		}) do
			makePart({
				Name = "ExtractionRing_" .. radius,
				Size = segment[1],
				CFrame = CFrame.new(segment[2]),
				Color = color,
				Material = Enum.Material.Neon,
				Transparency = 0.3,
				CanCollide = false,
				Parent = effects,
			})
		end
	end
	for _, x in ipairs({ -22, 22 }) do
		local finishPillar = makePart({
			Name = "FinishPillar",
			Size = Vector3.new(4, 24, 4),
			CFrame = CFrame.new(x, 13, finishZ),
			Color = FINISH_COLOR,
			Material = Enum.Material.Neon,
			CanCollide = false,
			Parent = effects,
		})
		addPointLight(finishPillar, FINISH_COLOR, 1.4, 24)
	end
	makePart({
		Name = "FinishTop",
		Size = Vector3.new(48, 4, 4),
		CFrame = CFrame.new(0, 27, finishZ),
		Color = FINISH_COLOR,
		Material = Enum.Material.Neon,
		Parent = effects,
	})
	local gate = makePart({
		Name = "FinishGate",
		Size = Vector3.new(Config.TrackWidth, 24, 1),
		CFrame = CFrame.new(0, 13, finishZ),
		Color = FINISH_COLOR,
		Material = Enum.Material.ForceField,
		Transparency = 0.55,
		CanCollide = false,
		CanTouch = false,
		Parent = effects,
	})
	addBillboard(gate, "EXTRACCION", FINISH_COLOR, 6, 360, 70)

	local finishTrigger = makePart({
		Name = "FinishTrigger",
		Size = Vector3.new(Config.TrackWidth, 24, 12),
		CFrame = CFrame.new(0, 13, finishZ),
		Transparency = 1,
		CanCollide = false,
		CanTouch = true,
		Parent = effects,
	})

	local voidPlane = makePart({
		Name = "VoidPlane",
		Size = Vector3.new(500, 24, Config.TrackLength + 400),
		CFrame = CFrame.new(0, -40, -Config.TrackLength / 2),
		Transparency = 1,
		CanCollide = false,
		CanTouch = true,
		Parent = effects,
	})

	-- Publicar la geometria antes de aplicar efectos para que el gameplay no dependa del postprocesado.
	map.Parent = Workspace

	-- Iluminacion y post-procesado.
	setLightingProperty("ClockTime", 18.5)
	setLightingProperty("Brightness", 1.85)
	setLightingProperty("Ambient", Color3.fromRGB(42, 48, 76))
	setLightingProperty("OutdoorAmbient", Color3.fromRGB(68, 76, 112))
	setLightingProperty("FogStart", 850)
	setLightingProperty("FogEnd", 3400)
	setLightingProperty("FogColor", Color3.fromRGB(78, 92, 140))
	setLightingProperty("GlobalShadows", true)
	setLightingProperty("ExposureCompensation", 0.05)
	setLightingProperty("EnvironmentDiffuseScale", 0.35)
	setLightingProperty("EnvironmentSpecularScale", 0.35)
	local sky = Lighting:FindFirstChildOfClass("Sky")
	if sky then
		pcall(function()
			sky.CelestialBodiesShown = true
			sky.StarCount = 3000
			sky.SunAngularSize = 12
			sky.MoonAngularSize = 8
		end)
	end

	local atmosphere = ensureEffect("Atmosphere", "SpeedEscapeAtmosphere")
	atmosphere.Density = 0.18
	atmosphere.Haze = 0.58
	atmosphere.Glare = 0
	atmosphere.Color = Color3.fromRGB(140, 170, 220)
	atmosphere.Decay = Color3.fromRGB(80, 100, 150)

	local bloom = ensureEffect("BloomEffect", "SpeedEscapeBloom")
	bloom.Intensity = 0.38
	bloom.Size = 24
	bloom.Threshold = 1.5

	local correction = ensureEffect("ColorCorrectionEffect", "SpeedEscapeColor")
	correction.Saturation = 0.1
	correction.Contrast = 0.12
	correction.Brightness = 0

	local rays = ensureEffect("SunRaysEffect", "SpeedEscapeRays")
	rays.Intensity = 0.03
	rays.Spread = 0.5

	-- Animacion: monedas girando y barras barriendo.
	task.spawn(function()
		local elapsed = 0
		while true do
			elapsed += task.wait(0.1)
			for _, entry in ipairs(coins) do
				local coin = entry.Part
				if coin.Parent and coin.CanTouch then
					local bob = math.sin(elapsed * 2 + entry.Position.Z * 0.05) * 0.25
					coin.CFrame = CFrame.new(entry.Position + Vector3.new(0, bob, 0))
						* CFrame.Angles(0, elapsed * 2.2 + entry.Position.Z * 0.1, 0)
						* CFrame.Angles(0, math.rad(90), 0)
				end
			end
		end
	end)

	task.spawn(function()
		while true do
			local deltaTime = task.wait(1 / 30)
			for _, sweeper in ipairs(sweepers) do
				if sweeper.Part.Parent then
					sweeper.Angle += deltaTime * sweeper.Speed
					sweeper.Part.CFrame = CFrame.new(sweeper.Position) * CFrame.Angles(0, sweeper.Angle, 0)
				end
			end
		end
	end)

	-- Pulso de la infraestructura: da vida a los hitos sin animar cientos de piezas.
	task.spawn(function()
		local elapsed = 0
		while true do
			elapsed += task.wait(1 / 20)
			local pulse = 0.18 + (math.sin(elapsed * 3.2) + 1) * 0.1
			for _, rail in ipairs(transitRails) do
				if rail.Parent then
					rail.Transparency = pulse
				end
			end
			for _, core in ipairs(reactorCores) do
				if core.Parent then
					core.Transparency = 0.08 + (math.sin(elapsed * 4.1) + 1) * 0.08
				end
			end
		end
	end)

	return {
		spawn = spawn,
		checkpoints = checkpoints,
		obstacles = obstacles,
		boostPads = boostPads,
		sweepers = sweepers,
		coins = coins,
		finishTrigger = finishTrigger,
		voidPlane = voidPlane,
		finishZ = finishZ,
	}
end

return MapBuilder
