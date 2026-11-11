-- HUD, tienda, misiones, ranking, combo, efectos y controles tactiles.
local Players = game:GetService("Players")
local ReplicatedStorage = game:GetService("ReplicatedStorage")
local RunService = game:GetService("RunService")
local TweenService = game:GetService("TweenService")
local UserInputService = game:GetService("UserInputService")
local Lighting = game:GetService("Lighting")
local MarketplaceService = game:GetService("MarketplaceService")

local player = Players.LocalPlayer
local playerGui = player:WaitForChild("PlayerGui")
local Config = require(ReplicatedStorage:WaitForChild("Shared"):WaitForChild("Config"))

player.CameraMode = Enum.CameraMode.Classic
player.CameraMinZoomDistance = 10
player.CameraMaxZoomDistance = 28

local Remotes = ReplicatedStorage:WaitForChild("Remotes")
local AddSpeed = Remotes:WaitForChild("AddSpeed")
local BuyUpgrade = Remotes:WaitForChild("BuyUpgrade")
local Rebirth = Remotes:WaitForChild("Rebirth")
local GetState = Remotes:WaitForChild("GetState")
local SyncState = Remotes:WaitForChild("SyncState")
local Notify = Remotes:WaitForChild("Notify")
local FX = Remotes:WaitForChild("FX")
local Leaderboard = Remotes:WaitForChild("Leaderboard")
local ClaimDaily = Remotes:WaitForChild("ClaimDaily")

local C = {
	bg = Color3.fromRGB(7, 9, 20),
	panel = Color3.fromRGB(15, 19, 38),
	panelLight = Color3.fromRGB(31, 38, 72),
	accent = Color3.fromRGB(80, 220, 255),
	violet = Color3.fromRGB(176, 112, 255),
	green = Color3.fromRGB(80, 255, 150),
	yellow = Color3.fromRGB(255, 215, 60),
	red = Color3.fromRGB(255, 90, 90),
	text = Color3.fromRGB(240, 244, 255),
	muted = Color3.fromRGB(150, 160, 190),
}
local FONT_BOLD = Enum.Font.GothamBold
local FONT_BLACK = Enum.Font.GothamBlack
local corePalette = { C.accent, Color3.fromRGB(255, 90, 220), C.yellow }

local function create(className, props)
	local instance = Instance.new(className)
	for key, value in pairs(props) do
		if key ~= "Parent" then
			instance[key] = value
		end
	end
	instance.Parent = props.Parent
	return instance
end

local function corner(parent, radius)
	local item = Instance.new("UICorner")
	item.CornerRadius = UDim.new(0, radius or 8)
	item.Parent = parent
end

local function stroke(parent, color, thickness, transparency)
	local item = Instance.new("UIStroke")
	item.Color = color or C.accent
	item.Thickness = thickness or 2
	item.Transparency = transparency or 0
	item.ApplyStrokeMode = Enum.ApplyStrokeMode.Border
	item.Parent = parent
end

local function gradient(parent, fromColor, toColor, rotation)
	local item = Instance.new("UIGradient")
	item.Color = ColorSequence.new(fromColor, toColor)
	item.Rotation = rotation or 90
	item.Parent = parent
	return item
end

local function comma(number)
	local text = tostring(math.floor(number))
	local count
	repeat
		text, count = text:gsub("^(-?%d+)(%d%d%d)", "%1,%2")
	until count == 0
	return text
end

local function short(number)
	number = math.floor(number)
	if number >= 1e9 then
		return string.format("%.1fB", number / 1e9)
	elseif number >= 1e6 then
		return string.format("%.1fM", number / 1e6)
	elseif number >= 1e3 then
		return string.format("%.1fK", number / 1e3)
	end
	return tostring(number)
end

local function clockText(seconds)
	seconds = math.max(0, math.floor(seconds or 0))
	return string.format("%02d:%02d", math.floor(seconds / 60), seconds % 60)
end

local function rankFor(speed)
	local selected = Config.RankTiers[1]
	for _, tier in ipairs(Config.RankTiers) do
		if speed >= tier.Speed then
			selected = tier
		end
	end
	return selected
end

local function zoneForZ(z)
	local index = math.clamp(math.floor((-z) / Config.ZoneLength) + 1, 1, #Config.Zones)
	return Config.Zones[index], index
end

local screenGui = create("ScreenGui", {
	Name = "NeonHeistHUD",
	IgnoreGuiInset = false,
	ResetOnSpawn = false,
	ZIndexBehavior = Enum.ZIndexBehavior.Sibling,
	DisplayOrder = 20,
	Parent = playerGui,
})

local backdrop = create("Frame", {
	Name = "Backdrop",
	Size = UDim2.fromScale(1, 1),
	BackgroundColor3 = C.bg,
	BackgroundTransparency = 0.72,
	BorderSizePixel = 0,
	ZIndex = 0,
	Parent = screenGui,
})
gradient(backdrop, Color3.fromRGB(7, 10, 28), Color3.fromRGB(27, 10, 42), 25)
local backgroundGrid = create("Frame", {
	Name = "BackgroundGrid",
	Size = UDim2.fromScale(1, 1),
	BackgroundTransparency = 1,
	BorderSizePixel = 0,
	ZIndex = 0,
	Parent = screenGui,
})
for index = -4, 4 do
	local line = create("Frame", {
		AnchorPoint = Vector2.new(0.5, 0.5),
		Position = UDim2.new(0.5, index * 190, 0.5, 0),
		Size = UDim2.new(0, 1, 1.5, 0),
		Rotation = 24,
		BackgroundColor3 = C.accent,
		BackgroundTransparency = 0.94,
		BorderSizePixel = 0,
		ZIndex = 0,
		Parent = backgroundGrid,
	})
	line.Name = "GridLine"
end
local zoneTint = create("Frame", {
	Name = "ZoneTint",
	Size = UDim2.fromScale(1, 1),
	BackgroundColor3 = Config.Zones[1].Accent,
	BackgroundTransparency = 0.96,
	BorderSizePixel = 0,
	ZIndex = 0,
	Parent = screenGui,
})

-- Estado local
local state = nil
local topData = {}
local currentSpeed = 0
local currentCoins = 0
local currentBest = 0
local displaySpeed = 0
local combo = 0
local heistRoundEndsAt = 0
local lastKeyTime = 0
local lastMobileSend = 0
local openName = nil
local togglePanel
local closePanel
local trail = nil

local speedBlur = Lighting:FindFirstChild("NeonHeistSpeedBlur")
if not speedBlur then
	speedBlur = Instance.new("BlurEffect")
	speedBlur.Name = "NeonHeistSpeedBlur"
	speedBlur.Parent = Lighting
end
speedBlur.Size = 0
local worldColor = Lighting:FindFirstChild("SpeedEscapeColor")

-- Velocidad (centro superior).
local speedFrame = create("Frame", {
	Name = "SpeedFrame",
	AnchorPoint = Vector2.new(0.5, 0),
	Position = UDim2.new(0.5, 0, 0, 10),
	Size = UDim2.fromOffset(340, 106),
	BackgroundColor3 = C.bg,
	BackgroundTransparency = 0.12,
	Parent = screenGui,
})
corner(speedFrame, 16)
stroke(speedFrame, C.accent, 2, 0.35)
gradient(speedFrame, Color3.fromRGB(13, 24, 48), Color3.fromRGB(20, 12, 42), 25)

create("TextLabel", {
	Position = UDim2.fromOffset(0, 6),
	Size = UDim2.new(1, 0, 0, 18),
	BackgroundTransparency = 1,
	Text = "VELOCIDAD",
	TextColor3 = C.muted,
	Font = FONT_BOLD,
	TextSize = 14,
	Parent = speedFrame,
})

local speedValue = create("TextLabel", {
	Position = UDim2.fromOffset(0, 20),
	Size = UDim2.new(1, 0, 0, 54),
	BackgroundTransparency = 1,
	Text = "0",
	TextColor3 = C.text,
	Font = FONT_BLACK,
	TextSize = 44,
	TextStrokeTransparency = 0.72,
	TextStrokeColor3 = Color3.new(0, 0, 0),
	Parent = speedFrame,
})

local rankLabel = create("TextLabel", {
	Position = UDim2.fromOffset(0, 76),
	Size = UDim2.new(1, 0, 0, 20),
	BackgroundTransparency = 1,
	Text = "NOVATO",
	TextColor3 = C.muted,
	Font = FONT_BLACK,
	TextSize = 14,
	Parent = speedFrame,
})

local comboLabel = create("TextLabel", {
	AnchorPoint = Vector2.new(0.5, 0),
	Position = UDim2.new(0.5, 0, 0, 120),
	Size = UDim2.fromOffset(420, 24),
	BackgroundTransparency = 1,
	Text = "COMBO x0",
	TextColor3 = C.muted,
	Font = FONT_BLACK,
	TextSize = 18,
	Parent = screenGui,
})

-- Barra de progreso.
local progressBackground = create("Frame", {
	AnchorPoint = Vector2.new(0.5, 0),
	Position = UDim2.new(0.5, 0, 0, 146),
	Size = UDim2.fromOffset(340, 12),
	BackgroundColor3 = C.panel,
	BorderSizePixel = 0,
	Parent = screenGui,
})
corner(progressBackground, 6)
stroke(progressBackground, C.panelLight, 1, 0.2)

local progressFill = create("Frame", {
	Size = UDim2.fromScale(0, 1),
	BackgroundColor3 = C.accent,
	BorderSizePixel = 0,
	Parent = progressBackground,
})
corner(progressFill, 6)
gradient(progressFill, C.accent, C.violet, 0)

local zoneLabel = create("TextLabel", {
	AnchorPoint = Vector2.new(0.5, 0),
	Position = UDim2.new(0.5, 0, 0, 162),
	Size = UDim2.fromOffset(420, 20),
	BackgroundTransparency = 1,
	Text = "ZONA 1 · " .. Config.Zones[1].Name,
	TextColor3 = C.accent,
	Font = FONT_BOLD,
	TextSize = 13,
	Parent = screenGui,
})

local heistLabel = create("TextLabel", {
	AnchorPoint = Vector2.new(0.5, 0),
	Position = UDim2.new(0.5, 0, 0, 182),
	Size = UDim2.fromOffset(420, 22),
	BackgroundTransparency = 1,
	Text = "NUCLEOS 0/3  ·  EXTRAE EN LA META",
	TextColor3 = C.yellow,
	Font = FONT_BLACK,
	TextSize = 13,
	Parent = screenGui,
})

local roundWarning = create("TextLabel", {
	AnchorPoint = Vector2.new(0.5, 0),
	Position = UDim2.new(0.5, 0, 0, 232),
	Size = UDim2.fromOffset(520, 26),
	BackgroundTransparency = 1,
	Text = "",
	TextColor3 = C.red,
	Font = FONT_BLACK,
	TextSize = 17,
	TextStrokeTransparency = 0.55,
	TextStrokeColor3 = Color3.new(0, 0, 0),
	Visible = false,
	Parent = screenGui,
})

-- Monedas, renacimientos y record.
local statsFrame = create("Frame", {
	Position = UDim2.fromOffset(14, 72),
	Size = UDim2.fromOffset(268, 88),
	BackgroundColor3 = C.bg,
	BackgroundTransparency = 0.12,
	Parent = screenGui,
})
corner(statsFrame, 14)
stroke(statsFrame, C.panelLight, 2, 0.2)
gradient(statsFrame, Color3.fromRGB(14, 21, 42), Color3.fromRGB(24, 14, 43), 20)

local brandFrame = create("Frame", {
	Name = "BrandFrame",
	Position = UDim2.fromOffset(14, 14),
	Size = UDim2.fromOffset(268, 48),
	BackgroundColor3 = C.panel,
	BackgroundTransparency = 0.08,
	BorderSizePixel = 0,
	Parent = screenGui,
})
corner(brandFrame, 14)
stroke(brandFrame, C.violet, 1, 0.2)
gradient(brandFrame, Color3.fromRGB(25, 18, 55), Color3.fromRGB(10, 32, 54), 0)
create("TextLabel", {
	Position = UDim2.fromOffset(14, 6),
	Size = UDim2.fromOffset(240, 22),
	BackgroundTransparency = 1,
	Text = "NEON HEIST",
	TextColor3 = C.text,
	Font = FONT_BLACK,
	TextSize = 20,
	TextXAlignment = Enum.TextXAlignment.Left,
	Parent = brandFrame,
})
create("TextLabel", {
	Position = UDim2.fromOffset(15, 28),
	Size = UDim2.fromOffset(240, 14),
	BackgroundTransparency = 1,
	Text = "CONTRATO 01  //  EXTRACCION",
	TextColor3 = C.violet,
	Font = FONT_BOLD,
	TextSize = 10,
	TextXAlignment = Enum.TextXAlignment.Left,
	Parent = brandFrame,
})

local function statBlock(parent, offsetX, title, color)
	create("TextLabel", {
		Position = UDim2.fromOffset(offsetX, 10),
		Size = UDim2.fromOffset(80, 18),
		BackgroundTransparency = 1,
		Text = title,
		TextColor3 = C.muted,
		Font = FONT_BOLD,
		TextSize = 11,
		TextXAlignment = Enum.TextXAlignment.Left,
		Parent = parent,
	})
	return create("TextLabel", {
		Position = UDim2.fromOffset(offsetX, 32),
		Size = UDim2.fromOffset(80, 32),
		BackgroundTransparency = 1,
		Text = "0",
		TextColor3 = color,
		Font = FONT_BLACK,
		TextSize = 21,
		TextXAlignment = Enum.TextXAlignment.Left,
		Parent = parent,
	})
end

local coinsLabel = statBlock(statsFrame, 14, "MONEDAS", C.yellow)
local rebirthsLabel = statBlock(statsFrame, 98, "RENACERES", C.green)
local bestLabel = statBlock(statsFrame, 182, "MEJOR", C.accent)

local contractCard = create("Frame", {
	Name = "ContractCard",
	AnchorPoint = Vector2.new(0, 1),
	Position = UDim2.new(0, 14, 1, -20),
	Size = UDim2.fromOffset(268, 76),
	BackgroundColor3 = C.panel,
	BackgroundTransparency = 0.06,
	BorderSizePixel = 0,
	Parent = screenGui,
})
corner(contractCard, 14)
stroke(contractCard, C.yellow, 1, 0.25)
gradient(contractCard, Color3.fromRGB(31, 25, 47), Color3.fromRGB(15, 23, 44), 0)
create("TextLabel", {
	Position = UDim2.fromOffset(14, 8),
	Size = UDim2.fromOffset(150, 16),
	BackgroundTransparency = 1,
	Text = "CONTRATO ACTIVO",
	TextColor3 = C.yellow,
	Font = FONT_BLACK,
	TextSize = 11,
	TextXAlignment = Enum.TextXAlignment.Left,
	Parent = contractCard,
})
local contractStatus = create("TextLabel", {
	Position = UDim2.fromOffset(14, 27),
	Size = UDim2.fromOffset(220, 18),
	BackgroundTransparency = 1,
	Text = "RECUPERA NUCLEOS Y EXTRAE",
	TextColor3 = C.text,
	Font = FONT_BOLD,
	TextSize = 12,
	TextXAlignment = Enum.TextXAlignment.Left,
	Parent = contractCard,
})
local contractProgress = create("TextLabel", {
	AnchorPoint = Vector2.new(1, 0),
	Position = UDim2.new(1, -14, 0, 8),
	Size = UDim2.fromOffset(64, 18),
	BackgroundTransparency = 1,
	Text = "0/3",
	TextColor3 = C.yellow,
	Font = FONT_BLACK,
	TextSize = 16,
	TextXAlignment = Enum.TextXAlignment.Right,
	Parent = contractCard,
})
local contractBar = create("Frame", {
	Position = UDim2.fromOffset(14, 55),
	Size = UDim2.new(1, -28, 0, 7),
	BackgroundColor3 = C.panelLight,
	BorderSizePixel = 0,
	Parent = contractCard,
})
corner(contractBar, 4)
local contractFill = create("Frame", {
	Size = UDim2.fromScale(0, 1),
	BackgroundColor3 = C.yellow,
	BorderSizePixel = 0,
	Parent = contractBar,
})
corner(contractFill, 4)
gradient(contractFill, C.yellow, C.violet, 0)

-- Entrada breve: comunica la fantasia del juego antes de soltar al jugador.
local intro = create("Frame", {
	Name = "Intro",
	Size = UDim2.fromScale(1, 1),
	BackgroundColor3 = C.bg,
	BackgroundTransparency = 0.12,
	BorderSizePixel = 0,
	ZIndex = 30,
	Parent = screenGui,
})
gradient(intro, Color3.fromRGB(8, 12, 32), Color3.fromRGB(39, 10, 47), 25)
local introPanel = create("Frame", {
	AnchorPoint = Vector2.new(0.5, 0.5),
	Position = UDim2.fromScale(0.5, 0.47),
	Size = UDim2.fromOffset(470, 260),
	BackgroundColor3 = C.panel,
	BackgroundTransparency = 0.04,
	BorderSizePixel = 0,
	ZIndex = 31,
	Parent = intro,
})
corner(introPanel, 22)
stroke(introPanel, C.accent, 2, 0.18)
gradient(introPanel, Color3.fromRGB(20, 26, 58), Color3.fromRGB(35, 12, 50), 20)
local introScale = Instance.new("UIScale")
introScale.Parent = introPanel
local function updateIntroScale()
	local camera = workspace.CurrentCamera
	local viewport = camera and camera.ViewportSize or Vector2.new(1280, 720)
	introScale.Scale = math.clamp(math.min((viewport.X - 30) / 470, (viewport.Y - 80) / 260), 0.62, 1)
end
updateIntroScale()
if workspace.CurrentCamera then
	workspace.CurrentCamera:GetPropertyChangedSignal("ViewportSize"):Connect(updateIntroScale)
end
create("TextLabel", {
	Position = UDim2.fromOffset(28, 24),
	Size = UDim2.new(1, -56, 0, 42),
	BackgroundTransparency = 1,
	Text = "NEON HEIST",
	TextColor3 = C.text,
	Font = FONT_BLACK,
	TextSize = 34,
	TextStrokeTransparency = 0.75,
	TextXAlignment = Enum.TextXAlignment.Left,
	ZIndex = 32,
	Parent = introPanel,
})
create("TextLabel", {
	Position = UDim2.fromOffset(30, 70),
	Size = UDim2.new(1, -60, 0, 22),
	BackgroundTransparency = 1,
	Text = "CONTRATO 01  //  EXTRACCION",
	TextColor3 = C.violet,
	Font = FONT_BOLD,
	TextSize = 13,
	TextXAlignment = Enum.TextXAlignment.Left,
	ZIndex = 32,
	Parent = introPanel,
})
create("TextLabel", {
	Position = UDim2.fromOffset(30, 108),
	Size = UDim2.new(1, -60, 0, 46),
	BackgroundTransparency = 1,
	Text = "Recupera 3 nucleos en orden.\nLlega a la meta y cobra la extraccion.",
	TextColor3 = C.text,
	Font = FONT_BOLD,
	TextSize = 17,
	TextWrapped = true,
	TextXAlignment = Enum.TextXAlignment.Left,
	ZIndex = 32,
	Parent = introPanel,
})
local introButton = create("TextButton", {
	AnchorPoint = Vector2.new(1, 1),
	Position = UDim2.new(1, -28, 1, -24),
	Size = UDim2.fromOffset(170, 46),
	BackgroundColor3 = C.accent,
	Text = "ENTRAR AL RUN",
	TextColor3 = C.bg,
	Font = FONT_BLACK,
	TextSize = 15,
	AutoButtonColor = false,
	ZIndex = 32,
	Parent = introPanel,
})
corner(introButton, 12)
gradient(introButton, C.accent, C.violet, 0)
local function closeIntro()
	if not intro.Visible then
		return
	end
	local tween = TweenService:Create(intro, TweenInfo.new(0.3), { BackgroundTransparency = 1 })
	local panelTween = TweenService:Create(introPanel, TweenInfo.new(0.25, Enum.EasingStyle.Back, Enum.EasingDirection.In), {
		Size = UDim2.fromOffset(430, 235),
	})
	tween:Play()
	panelTween:Play()
	task.delay(0.3, function()
		intro.Visible = false
	end)
end
introButton.Activated:Connect(closeIntro)
task.delay(4.5, closeIntro)

-- Ayuda inferior.
local isTouch = UserInputService.TouchEnabled
local hint = create("TextLabel", {
	AnchorPoint = Vector2.new(0.5, 1),
	Position = UDim2.new(0.5, 0, 1, -20),
	Size = UDim2.fromOffset(660, 26),
	BackgroundTransparency = 1,
	Text = "WASD para correr  |  Cualquier tecla carga tu escape",
	TextColor3 = C.muted,
	Font = FONT_BOLD,
	TextSize = 17,
	Visible = not isTouch,
	Parent = screenGui,
})

-- Botones de menu.
local navDock = create("Frame", {
	Name = "NavDock",
	AnchorPoint = Vector2.new(1, 1),
	Position = UDim2.new(1, -8, 1, -8),
	Size = UDim2.fromOffset(214, 286),
	BackgroundColor3 = C.panel,
	BackgroundTransparency = 0.2,
	BorderSizePixel = 0,
	Visible = not isTouch,
	Parent = screenGui,
})
corner(navDock, 18)
stroke(navDock, C.accent, 1, 0.68)
gradient(navDock, Color3.fromRGB(20, 24, 48), Color3.fromRGB(20, 11, 35), 25)
create("TextLabel", {
	Position = UDim2.fromOffset(16, 10),
	Size = UDim2.fromOffset(180, 14),
	BackgroundTransparency = 1,
	Text = "CONTROL DE OPERACION",
	TextColor3 = C.muted,
	Font = FONT_BOLD,
	TextSize = 10,
	TextXAlignment = Enum.TextXAlignment.Left,
	Parent = navDock,
})

local function menuButton(name, text, color, textColor)
	local button = create("TextButton", {
		Name = name,
		AnchorPoint = Vector2.new(1, 1),
		Position = UDim2.new(1, -16, 1, -18),
		Size = UDim2.fromOffset(190, 46),
		BackgroundColor3 = color,
		Text = text,
		TextColor3 = textColor,
		Font = FONT_BLACK,
		TextSize = 17,
		AutoButtonColor = false,
		Parent = screenGui,
	})
	corner(button, 12)
	stroke(button, textColor == C.bg and Color3.fromRGB(140, 240, 255) or C.panelLight, 1, 0.25)
	gradient(button, color:Lerp(Color3.new(1, 1, 1), 0.08), color:Lerp(Color3.new(0, 0, 0), 0.22), 90)
	button.MouseEnter:Connect(function()
		TweenService:Create(button, TweenInfo.new(0.12), {Size = UDim2.fromOffset(198, 48)}):Play()
	end)
	button.MouseLeave:Connect(function()
		TweenService:Create(button, TweenInfo.new(0.12), {Size = UDim2.fromOffset(190, 46)}):Play()
	end)
	return button
end

local rebirthButton = menuButton("RebirthButton", "RENACER", C.panelLight, C.muted)
stroke(rebirthButton, C.green, 2, 0.55)
local shopButton = menuButton("ShopButton", "TIENDA", C.accent, C.bg)
local missionsButton = menuButton("MissionsButton", "MISIONES", C.panelLight, C.text)
stroke(missionsButton, C.yellow, 2, 0.5)
local topButton = menuButton("TopButton", "TOP", C.panelLight, C.text)
stroke(topButton, C.accent, 2, 0.5)
local dailyButton = menuButton("DailyButton", "BONO DIARIO", C.yellow, C.bg)
local premiumButton = nil
local premiumPassId = Config.PassIds.DoubleSpeed > 0 and Config.PassIds.DoubleSpeed or Config.PassIds.DoubleCoins
if premiumPassId > 0 then
	premiumButton = menuButton("PremiumButton", "PASE DE OPERADOR", C.violet, C.text)
	stroke(premiumButton, C.yellow, 2, 0.35)
	navDock.Size = UDim2.fromOffset(214, 338)
end

local menuButtons = { rebirthButton, shopButton, missionsButton, topButton, dailyButton }
if premiumButton then
	table.insert(menuButtons, premiumButton)
end

local function layoutMenu()
	if isTouch then
		for index, button in ipairs(menuButtons) do
			button.AnchorPoint = Vector2.new(1, 0)
			button.Position = UDim2.new(1, -10, 0.34 + (index - 1) * 0.062, 0)
			button.Size = UDim2.fromOffset(128, 40)
			button.TextSize = 15
		end
	else
		for index, button in ipairs(menuButtons) do
			button.AnchorPoint = Vector2.new(1, 1)
			button.Position = UDim2.new(1, -16, 1, -18 - (index - 1) * 52)
			button.Size = UDim2.fromOffset(190, 46)
			button.TextSize = 17
		end
	end
end
layoutMenu()

-- Boton tactil +1.
local mobileButton = create("TextButton", {
	Name = "MobileSpeedButton",
	AnchorPoint = Vector2.new(0.5, 1),
	Position = UDim2.new(0.5, 0, 1, -18),
	Size = UDim2.fromOffset(116, 116),
	BackgroundColor3 = C.accent,
	BackgroundTransparency = 0.05,
	Text = "CARGAR\nESCAPE",
	TextColor3 = C.bg,
	Font = FONT_BLACK,
	TextSize = 22,
	TextWrapped = true,
	Visible = isTouch,
	Parent = screenGui,
})
corner(mobileButton, 58)
stroke(mobileButton, C.text, 2, 0.5)
gradient(mobileButton, Color3.fromRGB(100, 240, 255), Color3.fromRGB(106, 93, 255), 135)

-- Fondo de paneles.
local dimmer = create("TextButton", {
	Name = "Dimmer",
	Size = UDim2.fromScale(1, 1),
	BackgroundColor3 = Color3.new(0, 0, 0),
	BackgroundTransparency = 0.45,
	Text = "",
	AutoButtonColor = false,
	Visible = false,
	ZIndex = 10,
	Parent = screenGui,
})

local function makePanel(name, size, zIndex)
	local panel = create("Frame", {
		Name = name,
		AnchorPoint = Vector2.new(0.5, 0.5),
		Position = UDim2.fromScale(0.5, 0.5),
		Size = size,
		BackgroundColor3 = C.bg,
		Visible = false,
		ZIndex = zIndex,
		Parent = screenGui,
	})
	corner(panel, 18)
	stroke(panel, C.accent, 2, 0.2)
	gradient(panel, Color3.fromRGB(13, 18, 38), Color3.fromRGB(26, 12, 45), 25)

	local scale = Instance.new("UIScale")
	scale.Parent = panel
	local function updateScale()
		local camera = workspace.CurrentCamera
		local viewport = camera and camera.ViewportSize or Vector2.new(1280, 720)
		scale.Scale = math.clamp(math.min(
			(viewport.X - 36) / size.X.Offset,
			(viewport.Y - 36) / size.Y.Offset
		), 0.55, 1)
	end
	updateScale()
	if workspace.CurrentCamera then
		workspace.CurrentCamera:GetPropertyChangedSignal("ViewportSize"):Connect(updateScale)
	end
	return panel
end

local function addTitle(panel, text)
	create("TextLabel", {
		Position = UDim2.fromOffset(20, 14),
		Size = UDim2.new(1, -80, 0, 30),
		BackgroundTransparency = 1,
		Text = text,
		TextColor3 = C.text,
		Font = FONT_BLACK,
		TextSize = 22,
		TextXAlignment = Enum.TextXAlignment.Left,
		Parent = panel,
	})
end

local function addClose(panel)
	local button = create("TextButton", {
		AnchorPoint = Vector2.new(1, 0),
		Position = UDim2.new(1, -14, 0, 12),
		Size = UDim2.fromOffset(34, 34),
		BackgroundColor3 = C.panelLight,
		Text = "X",
		TextColor3 = C.text,
		Font = FONT_BLACK,
		TextSize = 16,
		Parent = panel,
	})
	corner(button, 8)
	gradient(button, C.panelLight, C.panel, 90)
	button.Activated:Connect(function()
		if closePanel then
			closePanel()
		end
	end)
end

-- Tienda.
local shopPanel = makePanel("ShopPanel", UDim2.fromOffset(500, 540), 11)
addTitle(shopPanel, "MEJORAS")
addClose(shopPanel)

local upgradeRows = {}
local upgradeOrder = { "Speed", "Coins", "Auto", "Magnet", "Jump" }

local function effectText(key)
	local upgrade = Config.Upgrades[key]
	if key == "Speed" or key == "Coins" then
		return string.format("+%d%% por nivel", math.floor(upgrade.Effect * 100))
	elseif key == "Auto" then
		return string.format("+%d velocidad por segundo", upgrade.Effect)
	elseif key == "Magnet" then
		return string.format("+%d studs de iman", upgrade.Effect)
	elseif key == "Jump" then
		return string.format("+%d de salto", upgrade.Effect)
	end
	return ""
end

for index, key in ipairs(upgradeOrder) do
	local upgrade = Config.Upgrades[key]
	local row = create("Frame", {
		Name = key .. "Row",
		Position = UDim2.fromOffset(16, 58 + (index - 1) * 86),
		Size = UDim2.new(1, -32, 0, 76),
		BackgroundColor3 = C.panel,
		BorderSizePixel = 0,
		Parent = shopPanel,
	})
	corner(row, 12)
	gradient(row, Color3.fromRGB(21, 27, 52), Color3.fromRGB(17, 17, 35), 0)

	create("TextLabel", {
		Position = UDim2.fromOffset(14, 7),
		Size = UDim2.fromOffset(250, 24),
		BackgroundTransparency = 1,
		Text = upgrade.Label,
		TextColor3 = C.text,
		Font = FONT_BLACK,
		TextSize = 19,
		TextXAlignment = Enum.TextXAlignment.Left,
		Parent = row,
	})

	local levelLabel = create("TextLabel", {
		Position = UDim2.fromOffset(14, 33),
		Size = UDim2.fromOffset(250, 18),
		BackgroundTransparency = 1,
		Text = "Nivel 0 / " .. upgrade.MaxLevel,
		TextColor3 = C.muted,
		Font = FONT_BOLD,
		TextSize = 13,
		TextXAlignment = Enum.TextXAlignment.Left,
		Parent = row,
	})

	create("TextLabel", {
		Position = UDim2.fromOffset(14, 52),
		Size = UDim2.fromOffset(270, 18),
		BackgroundTransparency = 1,
		Text = effectText(key),
		TextColor3 = C.muted,
		Font = FONT_BOLD,
		TextSize = 12,
		TextXAlignment = Enum.TextXAlignment.Left,
		Parent = row,
	})

	local buyButton = create("TextButton", {
		AnchorPoint = Vector2.new(1, 0.5),
		Position = UDim2.new(1, -14, 0.5, 0),
		Size = UDim2.fromOffset(146, 46),
		BackgroundColor3 = C.green,
		Text = "Comprar",
		TextColor3 = C.bg,
		Font = FONT_BLACK,
		TextSize = 15,
		AutoButtonColor = false,
		Parent = row,
	})
	corner(buyButton, 10)
	gradient(buyButton, C.green, Color3.fromRGB(34, 175, 125), 90)
	buyButton.Activated:Connect(function()
		BuyUpgrade:FireServer(key)
	end)

	upgradeRows[key] = { level = levelLabel, buy = buyButton }
end

-- Misiones.
local missionsPanel = makePanel("MissionsPanel", UDim2.fromOffset(480, 400), 11)
addTitle(missionsPanel, "MISIONES")
addClose(missionsPanel)

local missionRows = {}
local missionOrder = { "Keys", "Coins", "Checkpoint" }

for index, key in ipairs(missionOrder) do
	local definition = Config.Missions[key]
	local row = create("Frame", {
		Name = key .. "Mission",
		Position = UDim2.fromOffset(16, 58 + (index - 1) * 104),
		Size = UDim2.new(1, -32, 0, 92),
		BackgroundColor3 = C.panel,
		BorderSizePixel = 0,
		Parent = missionsPanel,
	})
	corner(row, 12)
	gradient(row, Color3.fromRGB(21, 27, 52), Color3.fromRGB(17, 17, 35), 0)

	create("TextLabel", {
		Position = UDim2.fromOffset(14, 8),
		Size = UDim2.fromOffset(240, 24),
		BackgroundTransparency = 1,
		Text = definition.Label,
		TextColor3 = C.text,
		Font = FONT_BLACK,
		TextSize = 18,
		TextXAlignment = Enum.TextXAlignment.Left,
		Parent = row,
	})

	local levelLabel = create("TextLabel", {
		AnchorPoint = Vector2.new(1, 0),
		Position = UDim2.new(1, -14, 0, 8),
		Size = UDim2.fromOffset(140, 22),
		BackgroundTransparency = 1,
		Text = "Nivel 0",
		TextColor3 = C.yellow,
		Font = FONT_BOLD,
		TextSize = 13,
		TextXAlignment = Enum.TextXAlignment.Right,
		Parent = row,
	})

	local barBackground = create("Frame", {
		Position = UDim2.fromOffset(14, 42),
		Size = UDim2.new(1, -28, 0, 10),
		BackgroundColor3 = C.panelLight,
		BorderSizePixel = 0,
		Parent = row,
	})
	corner(barBackground, 5)

	local barFill = create("Frame", {
		Size = UDim2.fromScale(0, 1),
		BackgroundColor3 = C.green,
		BorderSizePixel = 0,
		Parent = barBackground,
	})
	corner(barFill, 5)

	local progressLabel = create("TextLabel", {
		Position = UDim2.fromOffset(14, 57),
		Size = UDim2.fromOffset(220, 22),
		BackgroundTransparency = 1,
		Text = "0 / 0",
		TextColor3 = C.muted,
		Font = FONT_BOLD,
		TextSize = 12,
		TextXAlignment = Enum.TextXAlignment.Left,
		Parent = row,
	})

	local rewardLabel = create("TextLabel", {
		AnchorPoint = Vector2.new(1, 0),
		Position = UDim2.new(1, -14, 0, 57),
		Size = UDim2.fromOffset(220, 22),
		BackgroundTransparency = 1,
		Text = "",
		TextColor3 = C.yellow,
		Font = FONT_BOLD,
		TextSize = 12,
		TextXAlignment = Enum.TextXAlignment.Right,
		Parent = row,
	})

	missionRows[key] = {
		level = levelLabel,
		fill = barFill,
		progress = progressLabel,
		reward = rewardLabel,
	}
end

-- Ranking.
local topPanel = makePanel("TopPanel", UDim2.fromOffset(420, 540), 11)
addTitle(topPanel, "TOP DE LA PARTIDA")
addClose(topPanel)

local topContainer = create("Frame", {
	Position = UDim2.fromOffset(16, 58),
	Size = UDim2.new(1, -32, 1, -74),
	BackgroundTransparency = 1,
	ClipsDescendants = true,
	Parent = topPanel,
})

-- Avisos.
local toastFrame = create("Frame", {
	Name = "Toast",
	AnchorPoint = Vector2.new(0.5, 0),
	Position = UDim2.new(0.5, 0, 0, 236),
	Size = UDim2.fromOffset(500, 44),
	BackgroundColor3 = C.panel,
	BackgroundTransparency = 0.05,
	Visible = false,
	ZIndex = 20,
	Parent = screenGui,
})
corner(toastFrame, 10)
stroke(toastFrame, C.accent, 1, 0.35)

local toastLabel = create("TextLabel", {
	Position = UDim2.fromOffset(12, 0),
	Size = UDim2.new(1, -24, 1, 0),
	BackgroundTransparency = 1,
	Text = "",
	TextColor3 = C.text,
	Font = FONT_BOLD,
	TextSize = 14,
	TextWrapped = true,
	Parent = toastFrame,
})

local toastQueue = {}
local toastBusy = false

local function showToast(message)
	table.insert(toastQueue, message)
	if toastBusy then
		return
	end
	toastBusy = true
	task.spawn(function()
		while #toastQueue > 0 do
			local text = table.remove(toastQueue, 1)
			toastLabel.Text = text
			toastFrame.Visible = true
			toastFrame.BackgroundTransparency = 0.05
			toastFrame.Position = UDim2.new(0.5, 0, 0, 250)
			TweenService:Create(toastFrame, TweenInfo.new(0.15), {
				Position = UDim2.new(0.5, 0, 0, 258),
			}):Play()
			task.wait(1.7)
			TweenService:Create(toastFrame, TweenInfo.new(0.2), {
				Position = UDim2.new(0.5, 0, 0, 246),
				BackgroundTransparency = 1,
			}):Play()
			task.wait(0.2)
		end
		toastFrame.Visible = false
		toastFrame.BackgroundTransparency = 0.05
		toastBusy = false
	end)
end

-- Banner de zona.
local zoneBanner = create("TextLabel", {
	AnchorPoint = Vector2.new(0.5, 0),
	Position = UDim2.new(0.5, 0, 0, 208),
	Size = UDim2.fromOffset(560, 42),
	BackgroundTransparency = 1,
	Text = "",
	TextColor3 = C.accent,
	Font = FONT_BLACK,
	TextSize = 26,
	TextStrokeTransparency = 0.4,
	TextStrokeColor3 = Color3.new(0, 0, 0),
	TextTransparency = 1,
	Parent = screenGui,
})
local zoneToken = 0

local function showZone(index)
	local zone = Config.Zones[index]
	zoneBanner.Text = "ZONA " .. index .. " · " .. zone.Name
	zoneBanner.TextColor3 = zone.Accent
	zoneTint.BackgroundColor3 = zone.Accent
	zoneTint.BackgroundTransparency = 0.9
	TweenService:Create(zoneTint, TweenInfo.new(1.2), { BackgroundTransparency = 0.96 }):Play()
	worldColor = worldColor or Lighting:FindFirstChild("SpeedEscapeColor")
	if worldColor then
		local tint = zone.Accent:Lerp(Color3.new(1, 1, 1), 0.78)
		TweenService:Create(worldColor, TweenInfo.new(1.5), { TintColor = tint }):Play()
	end
	zoneToken += 1
	local token = zoneToken
	zoneBanner.TextTransparency = 1
	TweenService:Create(zoneBanner, TweenInfo.new(0.25), { TextTransparency = 0 }):Play()
	task.delay(2.2, function()
		if token ~= zoneToken then
			return
		end
		TweenService:Create(zoneBanner, TweenInfo.new(0.4), { TextTransparency = 1 }):Play()
	end)
end

-- Flash de pantalla.
local flash = create("Frame", {
	Name = "Flash",
	Size = UDim2.fromScale(1, 1),
	BackgroundColor3 = C.red,
	BackgroundTransparency = 1,
	BorderSizePixel = 0,
	ZIndex = 40,
	Parent = screenGui,
})

local function flashScreen(color, peak)
	flash.BackgroundColor3 = color
	flash.BackgroundTransparency = 0.75
	TweenService:Create(flash, TweenInfo.new(0.08), { BackgroundTransparency = peak }):Play()
	task.delay(0.1, function()
		TweenService:Create(flash, TweenInfo.new(0.3), { BackgroundTransparency = 1 }):Play()
	end)
end

-- Lineas de velocidad muy sutiles: solo aparecen cuando el jugador ya va rapido.
local speedLines = {}
for index = 1, 8 do
		local line = create("Frame", {
			Name = "SpeedLine_" .. index,
			Size = UDim2.fromOffset(150 + index * 12, 2),
			Position = UDim2.fromScale((index * 0.137) % 1, 0.2 + ((index * 0.083) % 0.62)),
			BackgroundColor3 = index % 2 == 0 and C.accent or C.violet,
			BackgroundTransparency = 1,
			BorderSizePixel = 0,
			Rotation = index % 2 == 0 and -8 or 8,
			ZIndex = 1,
			Parent = screenGui,
		})
		table.insert(speedLines, line)
end

-- Textos.
local function updateSpeedText()
	speedValue.Text = comma(displaySpeed)
end

local function updateComboText()
	if combo <= 0 then
		comboLabel.Text = "COMBO x0"
		comboLabel.TextColor3 = C.muted
	else
		local bonus = math.floor((Config.GetComboMultiplier(combo) - 1) * 100)
		comboLabel.Text = string.format("COMBO x%d  ·  +%d%% monedas", combo, bonus)
		comboLabel.TextColor3 = combo >= 50 and C.yellow or C.accent
	end
end

local function setCurrentSpeed(value)
	currentSpeed = value
	local rank = rankFor(value)
	rankLabel.Text = rank.Name
	rankLabel.TextColor3 = rank.Color
	if trail then
		trail.Color = ColorSequence.new(rank.Color)
	end
end

local function updateRebirthButton()
	local canRebirth = currentSpeed >= Config.RebirthRequirement
	rebirthButton.Text = "RENACER " .. short(Config.RebirthRequirement)
	rebirthButton.BackgroundColor3 = canRebirth and C.green or C.panelLight
	rebirthButton.TextColor3 = canRebirth and C.bg or C.muted
end

local function renderShop()
	if not state then
		return
	end
	for key, row in pairs(upgradeRows) do
		local upgrade = Config.Upgrades[key]
		local level = (state.Upgrades and state.Upgrades[key]) or 0
		row.level.Text = string.format("Nivel %d / %d", level, upgrade.MaxLevel)
		if level >= upgrade.MaxLevel then
			row.buy.Text = "MAXIMO"
			row.buy.BackgroundColor3 = C.panelLight
			row.buy.TextColor3 = C.muted
		else
			local cost = Config.GetUpgradeCost(key, level)
			row.buy.Text = "Comprar " .. short(cost)
			if currentCoins >= cost then
				row.buy.BackgroundColor3 = C.green
				row.buy.TextColor3 = C.bg
			else
				row.buy.BackgroundColor3 = C.panelLight
				row.buy.TextColor3 = C.muted
			end
		end
	end
end

local function renderMissions()
	if not state or not state.Missions then
		return
	end
	for key, row in pairs(missionRows) do
		local mission = state.Missions[key] or { Level = 0, Progress = 0 }
		local target = Config.GetMissionTarget(key, mission.Level)
		local progress = math.min(mission.Progress, target)
		row.level.Text = "Nivel " .. mission.Level
		row.progress.Text = short(progress) .. " / " .. short(target)
		row.reward.Text = "Recompensa: " .. short(Config.GetMissionReward(key, mission.Level))
		row.fill.Size = UDim2.fromScale(target > 0 and progress / target or 0, 1)
	end
end

local function renderTop()
	for _, child in ipairs(topContainer:GetChildren()) do
		child:Destroy()
	end
	if #topData == 0 then
		create("TextLabel", {
			Size = UDim2.new(1, 0, 0, 40),
			BackgroundTransparency = 1,
			Text = "Aun no hay records. Se el primero.",
			TextColor3 = C.muted,
			Font = FONT_BOLD,
			TextSize = 14,
			Parent = topContainer,
		})
		return
	end
	for index, entry in ipairs(topData) do
		local isMe = entry.Name == player.Name
		local row = create("Frame", {
			Position = UDim2.fromOffset(0, (index - 1) * 42),
			Size = UDim2.new(1, 0, 0, 36),
			BackgroundColor3 = isMe and C.panelLight or C.panel,
			BorderSizePixel = 0,
			Parent = topContainer,
		})
		corner(row, 8)
		if isMe then
			stroke(row, C.accent, 1, 0)
		end
		create("TextLabel", {
			Position = UDim2.fromOffset(12, 0),
			Size = UDim2.fromOffset(40, 36),
			BackgroundTransparency = 1,
			Text = "#" .. index,
			TextColor3 = index <= 3 and C.yellow or C.muted,
			Font = FONT_BLACK,
			TextSize = 15,
			TextXAlignment = Enum.TextXAlignment.Left,
			Parent = row,
		})
		create("TextLabel", {
			Position = UDim2.fromOffset(54, 0),
			Size = UDim2.new(1, -160, 1, 0),
			BackgroundTransparency = 1,
			Text = entry.DisplayName or entry.Name or "Jugador",
			TextColor3 = isMe and C.accent or C.text,
			Font = FONT_BOLD,
			TextSize = 14,
			TextXAlignment = Enum.TextXAlignment.Left,
			TextTruncate = Enum.TextTruncate.AtEnd,
			Parent = row,
		})
		create("TextLabel", {
			AnchorPoint = Vector2.new(1, 0),
			Position = UDim2.new(1, -12, 0, 0),
			Size = UDim2.fromOffset(100, 36),
			BackgroundTransparency = 1,
			Text = short(entry.Value or 0),
			TextColor3 = C.green,
			Font = FONT_BLACK,
			TextSize = 15,
			TextXAlignment = Enum.TextXAlignment.Right,
			Parent = row,
		})
	end
end

local function applyState(newState)
	if type(newState) ~= "table" then
		return
	end
	state = newState
	state.Upgrades = state.Upgrades or {}
	state.Missions = state.Missions or {}
	heistRoundEndsAt = state.HeistRoundEndsAt or heistRoundEndsAt
	local heistCores = state.HeistCores or 0
	local heistGoal = state.HeistGoal or Config.HeistCoreCount
	local remaining = heistRoundEndsAt - workspace:GetServerTimeNow()
	heistLabel.Text = string.format("NUCLEOS %d/%d  ·  RUN %s", heistCores, heistGoal, clockText(remaining))
	heistLabel.TextColor3 = remaining <= 15 and C.red or C.yellow
	contractStatus.Text = string.format("EXTRAE ANTES DE %s", clockText(remaining))
	contractStatus.TextColor3 = remaining <= 15 and C.red or C.text
	roundWarning.Visible = remaining > 0 and remaining <= 10
	roundWarning.Text = roundWarning.Visible and ("EXTRACCION URGENTE  //  " .. clockText(remaining)) or ""
	contractProgress.Text = string.format("%d/%d", heistCores, heistGoal)
	contractFill.Size = UDim2.fromScale(heistGoal > 0 and math.clamp(heistCores / heistGoal, 0, 1) or 0, 1)
	local nextCoreColor = corePalette[math.min(heistCores + 1, #corePalette)] or C.yellow
	contractProgress.TextColor3 = nextCoreColor
	contractFill.BackgroundColor3 = nextCoreColor
	local dailyStreak = math.clamp(state.DailyStreak or 0, 0, 7)
	local dailyClaimed = state.LastDailyClaim == os.date("!%Y-%m-%d")
	dailyButton.Text = dailyClaimed and ("BONO COBRADO  ·  " .. dailyStreak .. "/7")
		or ("BONO DIARIO  ·  DIA " .. math.min(dailyStreak + 1, 7))
	if type(state.Speed) == "number" then
		displaySpeed = math.max(displaySpeed, state.Speed)
		setCurrentSpeed(state.Speed)
		updateSpeedText()
	end
	if type(state.Best) == "number" then
		currentBest = math.max(currentBest, state.Best)
		bestLabel.Text = short(currentBest)
	end
	renderShop()
	renderMissions()
	updateRebirthButton()
end

togglePanel = function(name)
	local target = (openName ~= name) and name or nil
	openName = target
	dimmer.Visible = target ~= nil
	shopPanel.Visible = target == "shop"
	missionsPanel.Visible = target == "missions"
	topPanel.Visible = target == "top"
	if target == "shop" then
		renderShop()
	elseif target == "missions" then
		renderMissions()
	elseif target == "top" then
		renderTop()
	end
end

closePanel = function()
	togglePanel(nil)
end

shopButton.Activated:Connect(function()
	togglePanel("shop")
end)
missionsButton.Activated:Connect(function()
	togglePanel("missions")
end)
topButton.Activated:Connect(function()
	togglePanel("top")
end)
dailyButton.Activated:Connect(function()
	ClaimDaily:FireServer()
end)
if premiumButton then
	premiumButton.Activated:Connect(function()
		MarketplaceService:PromptGamePassPurchase(player, premiumPassId)
	end)
end
rebirthButton.Activated:Connect(function()
	Rebirth:FireServer()
end)
dimmer.Activated:Connect(function()
	closePanel()
end)

-- Particulas y estela.
local burstEmitter = nil

local function ensureEmitter()
	local character = player.Character
	local root = character and character:FindFirstChild("HumanoidRootPart")
	if not root then
		return nil
	end
	if burstEmitter and burstEmitter.Parent == root then
		return burstEmitter
	end
	if burstEmitter then
		burstEmitter:Destroy()
	end
	burstEmitter = Instance.new("ParticleEmitter")
	burstEmitter.Name = "Burst"
	burstEmitter.Lifetime = NumberRange.new(0.35, 0.7)
	burstEmitter.Speed = NumberRange.new(8, 16)
	burstEmitter.SpreadAngle = Vector2.new(180, 180)
	burstEmitter.Rate = 0
	burstEmitter.Size = NumberSequence.new({
		NumberSequenceKeypoint.new(0, 0.6),
		NumberSequenceKeypoint.new(1, 0),
	})
	burstEmitter.Transparency = NumberSequence.new({
		NumberSequenceKeypoint.new(0, 0.2),
		NumberSequenceKeypoint.new(1, 1),
	})
	burstEmitter.LightEmission = 1
	burstEmitter.Parent = root
	return burstEmitter
end

local function emitBurst(color, count)
	local emitter = ensureEmitter()
	if not emitter then
		return
	end
	emitter.Color = ColorSequence.new(color)
	emitter:Emit(count)
end

local function setupTrail(character)
	local root = character:WaitForChild("HumanoidRootPart", 10)
	if not root then
		return
	end
	if trail then
		trail:Destroy()
	end
	local attachment0 = Instance.new("Attachment")
	attachment0.Position = Vector3.new(0.6, 0, 0.6)
	attachment0.Parent = root
	local attachment1 = Instance.new("Attachment")
	attachment1.Position = Vector3.new(-0.6, 0, 0.6)
	attachment1.Parent = root
	trail = Instance.new("Trail")
	trail.Attachment0 = attachment0
	trail.Attachment1 = attachment1
	trail.Lifetime = 0.35
	trail.MinLength = 0.1
	trail.WidthScale = NumberSequence.new(0.7)
	trail.Transparency = NumberSequence.new({
		NumberSequenceKeypoint.new(0, 0.15),
		NumberSequenceKeypoint.new(1, 1),
	})
	trail.LightEmission = 0.8
	trail.Color = ColorSequence.new(rankFor(currentSpeed).Color)
	trail.Parent = root
end

player.CharacterAdded:Connect(function(character)
	task.wait(0.2)
	setupTrail(character)
	ensureEmitter()
end)
if player.Character then
	task.spawn(function()
		task.wait(0.2)
		setupTrail(player.Character)
		ensureEmitter()
	end)
end

-- Camara.
local shake = 0

local function applyShake(deltaTime)
	if shake <= 0 then
		return
	end
	local camera = workspace.CurrentCamera
	if not camera then
		return
	end
	local amount = shake * 0.6
	camera.CFrame = camera.CFrame
		* CFrame.new((math.random() - 0.5) * amount, (math.random() - 0.5) * amount, 0)
		* CFrame.Angles(0, 0, (math.random() - 0.5) * shake * 0.02)
	shake = math.max(0, shake - deltaTime * 8)
end

FX.OnClientEvent:Connect(function(kind, fxColor)
	if kind == "Choque" then
		flashScreen(C.red, 0.45)
		shake = 1
	elseif kind == "Boost" then
		emitBurst(rankFor(currentSpeed).Color, 30)
		flashScreen(C.accent, 0.8)
	elseif kind == "Meta" then
		flashScreen(C.yellow, 0.4)
		emitBurst(C.yellow, 60)
		shake = 0.5
	elseif kind == "Renacer" then
		displaySpeed = 0
		currentSpeed = 0
		updateSpeedText()
		setCurrentSpeed(0)
		updateRebirthButton()
		flashScreen(C.green, 0.35)
		emitBurst(C.green, 80)
		shake = 0.7
	elseif kind == "Mision" then
		emitBurst(C.yellow, 20)
	elseif kind == "Moneda" then
		emitBurst(C.yellow, 6)
	elseif kind == "HeistCore" then
		local color = typeof(fxColor) == "Color3" and fxColor or C.yellow
		emitBurst(color, 45)
		flashScreen(color, 0.6)
	elseif kind == "RoundReset" then
		flashScreen(C.violet, 0.4)
		emitBurst(C.violet, 55)
		shake = 0.35
	elseif kind == "Daily" then
		flashScreen(C.yellow, 0.35)
		emitBurst(C.yellow, 90)
		shake = 0.4
	end
end)

Leaderboard.OnClientEvent:Connect(function(list)
	if type(list) ~= "table" then
		return
	end
	topData = list
	if openName == "top" then
		renderTop()
	end
end)

SyncState.OnClientEvent:Connect(applyState)
Notify.OnClientEvent:Connect(showToast)

-- Input de teclado: refleja combo y velocidad al instante.
local function registerKey()
	local now = os.clock()
	if now - lastKeyTime <= Config.ComboWindow then
		combo = math.min(Config.ComboMax, combo + 1)
	else
		combo = 1
	end
	lastKeyTime = now
	displaySpeed += 1
	updateSpeedText()
	updateComboText()
end

UserInputService.InputBegan:Connect(function(input, gameProcessed)
	if gameProcessed or input.UserInputType ~= Enum.UserInputType.Keyboard then
		return
	end
	registerKey()
end)

-- Boton tactil: mantener pulsado dispara +1.
local mobileHeld = false

local function pressMobile()
	local now = os.clock()
	registerKey()
	if now - lastMobileSend >= 1 / 30 then
		lastMobileSend = now
		AddSpeed:FireServer()
	end
end

mobileButton.InputBegan:Connect(function(input)
	if input.UserInputType ~= Enum.UserInputType.Touch and input.UserInputType ~= Enum.UserInputType.MouseButton1 then
		return
	end
	if mobileHeld then
		return
	end
	mobileHeld = true
	pressMobile()
	task.spawn(function()
		while mobileHeld do
			task.wait(1 / 30)
			if mobileHeld then
				pressMobile()
			end
		end
	end)
end)

mobileButton.InputEnded:Connect(function()
	mobileHeld = false
end)

UserInputService.InputEnded:Connect(function(input)
	if input.UserInputType == Enum.UserInputType.Touch or input.UserInputType == Enum.UserInputType.MouseButton1 then
		mobileHeld = false
	end
end)

-- Stats del servidor.
local leaderstats = player:WaitForChild("leaderstats")
local speedStat = leaderstats:WaitForChild("Speed")
local coinsStat = leaderstats:WaitForChild("Coins")
local rebirthStat = leaderstats:WaitForChild("Rebirths")
local bestStat = leaderstats:WaitForChild("Best")

setCurrentSpeed(speedStat.Value)
displaySpeed = speedStat.Value
updateSpeedText()
updateComboText()
updateRebirthButton()

speedStat.Changed:Connect(function(value)
	setCurrentSpeed(value)
	if value > displaySpeed or displaySpeed - value > 8 then
		displaySpeed = value
		updateSpeedText()
	end
	updateRebirthButton()
end)

coinsStat.Changed:Connect(function(value)
	currentCoins = value
	coinsLabel.Text = short(value)
	renderShop()
end)

rebirthStat.Changed:Connect(function(value)
	rebirthsLabel.Text = tostring(value)
end)

bestStat.Changed:Connect(function(value)
	currentBest = math.max(currentBest, value)
	bestLabel.Text = short(currentBest)
end)

currentCoins = coinsStat.Value
coinsLabel.Text = short(currentCoins)
rebirthsLabel.Text = tostring(rebirthStat.Value)
currentBest = math.max(currentBest, bestStat.Value)
bestLabel.Text = short(currentBest)

task.spawn(function()
	for _ = 1, 12 do
		local ok, result = pcall(function()
			return GetState:InvokeServer()
		end)
		if ok and type(result) == "table" then
			applyState(result)
			return
		end
		task.wait(0.5)
	end
end)

local lastZoneIndex = 0
local lastRankName = ""

RunService.RenderStepped:Connect(function(deltaTime)
	local character = player.Character
	local root = character and character:FindFirstChild("HumanoidRootPart")
	if root then
		local progress = math.clamp(-root.Position.Z / Config.TrackLength, 0, 1)
		progressFill.Size = UDim2.fromScale(progress, 1)
		local zone, zoneIndex = zoneForZ(root.Position.Z)
		if zoneIndex ~= lastZoneIndex then
			lastZoneIndex = zoneIndex
			zoneLabel.Text = string.format("ZONA %d · %s", zoneIndex, zone.Name)
			zoneLabel.TextColor3 = zone.Accent
			progressFill.BackgroundColor3 = zone.Accent
			showZone(zoneIndex)
		end
	end
	if state and heistRoundEndsAt > 0 then
		local remaining = heistRoundEndsAt - workspace:GetServerTimeNow()
		local heistCores = state.HeistCores or 0
		local heistGoal = state.HeistGoal or Config.HeistCoreCount
		heistLabel.Text = string.format("NUCLEOS %d/%d  ·  RUN %s", heistCores, heistGoal, clockText(remaining))
		heistLabel.TextColor3 = remaining <= 15 and C.red or C.yellow
		contractStatus.Text = string.format("EXTRAE ANTES DE %s", clockText(remaining))
		contractStatus.TextColor3 = remaining <= 15 and C.red or C.text
		roundWarning.Visible = remaining > 0 and remaining <= 10
		roundWarning.Text = roundWarning.Visible and ("EXTRACCION URGENTE  //  " .. clockText(remaining)) or ""
	end

	if combo > 0 and os.clock() - lastKeyTime > Config.ComboWindow then
		combo = 0
		updateComboText()
	end

	local rank = rankFor(currentSpeed)
	if rank.Name ~= lastRankName then
		if lastRankName ~= "" then
			flashScreen(rank.Color, 0.82)
			emitBurst(rank.Color, 36)
			showToast("RANGO DESBLOQUEADO  //  " .. rank.Name)
		end
		lastRankName = rank.Name
		if trail then
			trail.Color = ColorSequence.new(rank.Color)
		end
	end

	local camera = workspace.CurrentCamera
	if camera then
		local targetFov = 70 + math.min(30, math.log10(1 + currentSpeed) * 7)
		camera.FieldOfView += (targetFov - camera.FieldOfView) * math.min(1, deltaTime * 4)
	end
	local targetBlur = math.clamp((currentSpeed - 300) / 350, 0, 6)
	speedBlur.Size += (targetBlur - speedBlur.Size) * math.min(1, deltaTime * 5)
	local speedFactor = math.clamp((currentSpeed - 140) / 800, 0, 1)
	local now = os.clock()
	for index, line in ipairs(speedLines) do
		line.Visible = speedFactor > 0
		if line.Visible then
			local flow = (now * (0.08 + speedFactor * 0.75) + index * 0.13) % 1
			line.Position = UDim2.fromScale((index * 0.17 + flow * 0.72) % 1, 0.18 + ((index * 0.083) % 0.66))
			line.BackgroundTransparency = 0.98 - speedFactor * 0.2
		end
	end

	applyShake(deltaTime)
end)
