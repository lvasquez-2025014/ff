-- Envia +1 velocidad al servidor con cada tecla pulsada (con limite local).
local Players = game:GetService("Players")
local UserInputService = game:GetService("UserInputService")
local ReplicatedStorage = game:GetService("ReplicatedStorage")

local player = Players.LocalPlayer

local Remotes = ReplicatedStorage:WaitForChild("Remotes")
local AddSpeed = Remotes:WaitForChild("AddSpeed")

local MIN_INTERVAL = 1 / 30
local lastSent = 0

UserInputService.InputBegan:Connect(function(input, gameProcessed)
	if gameProcessed then
		return
	end
	if input.UserInputType ~= Enum.UserInputType.Keyboard then
		return
	end
	local now = os.clock()
	if now - lastSent < MIN_INTERVAL then
		return
	end
	lastSent = now
	AddSpeed:FireServer()
end)
