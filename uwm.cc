
-- [+] Fixed Script
-- [+] Fixed FOV
-- [+] Fixed Untargetting inside FOV (probably)
-- [+] Enable / Disabled Notification Toggle
-- [+] * Prediction
-- [+] Horizontal
-- [+] Vertical
-- [+] Added Prediction Adjustment
-- [+] * Targeting
-- [+] TargetMode ["Closest" & "HitPart"]
-- [+] UntargetOnFall
-- [+] HitPart ["Head", "Torso", "HumanoidRootPart", "LeftArm", "RightArm", "LeftLeg", "RightLeg", "LowerRightLeg", "LowerLeftLeg"]
-- [+] * Added Dynamic FOV
-- [+] BasedOn ["Health" & "Speed"]
-- [+] MinFovRadius
-- [+] MaxFovRadius
-- [+] SpeedThreshold
-- [+] HealthThreshold
-- [+] * Humanization
-- [+] HumanizationFactor
-- [+] TargetSwitchDuration
-- [+] * Watermark
-- [+] Enable / Disable
-- [+] Sound
-- [+] Music
-- [+] Fixed ATON OF SHIT while making this
--[[ Disclaimer: 
if your getting banned using togglecat576's Silent, while da hood updated and,
its not my fault or anyones fault in my team your using Cheats at your OWN RISK,
so be careful and check for Da Hood updates before using it.
--]]
--[[ Copying: 
please dont just copy anything without credits, if you wanna make a better remake,
or anything atleast give me credits and dont remove the watermark, read the README file.
--]]

-- // Services // -- made by togglecat576 ://
local G = game
local Run_Service = G:GetService("RunService")
local Players = G:GetService("Players")
local UserInputService = G:GetService("UserInputService")
local Local_Player = Players.LocalPlayer
local Mouse = Local_Player:GetMouse()
local Current_Camera = G:GetService("Workspace").CurrentCamera
local Replicated_Storage = G:GetService("ReplicatedStorage")
local StarterGui = G:GetService("StarterGui")
local Workspace = G:GetService("Workspace")
local SoundService = G:GetService("SoundService")

-- // Variables // -- made by togglecat576 :///
local Target = nil
local V2 = Vector2.new
local Fov = Drawing.new("Circle")
local lastToolUse = 0
local HitPoint = Drawing.new("Circle")
local FovParts = {}  -- Store the parts for square and triangle FOV
local wallCheckFrequency = 10  -- Perform wall check every 10 frames
local frameCounter = 0
local lastSwitchTime = tick()
local fallStartTime = {}

-- // Game Load Check // -- made by togglecat576 ://
if not game:IsLoaded() then
    game.Loaded:Wait()
end

-- // Clear FOV Parts // -- made by togglecat576 ://
local function clearFovParts()
    for _, part in pairs(FovParts) do
        part:Remove()
    end
    FovParts = {}
end

-- // Update FOV Function // -- made by togglecat576 ://
local function updateFov()
    local settings = getgenv().Togglecat576Silent.FovSettings
    clearFovParts()
    if settings.DynamicFOV.Enabled then
        local dynamicSettings = settings.DynamicFOV
        if dynamicSettings.Type == "Speed" and Local_Player.Character and Local_Player.Character:FindFirstChild("HumanoidRootPart") then
            local speed = Local_Player.Character.HumanoidRootPart.Velocity.Magnitude
            settings.FovRadius = math.clamp(dynamicSettings.MinFOV + (speed / dynamicSettings.SpeedThreshold) * (dynamicSettings.MaxFOV - dynamicSettings.MinFOV), dynamicSettings.MinFOV, dynamicSettings.MaxFOV)
        elseif dynamicSettings.Type == "Health" and Local_Player.Character and Local_Player.Character:FindFirstChild("Humanoid") then
            local healthPercent = (Local_Player.Character.Humanoid.Health / Local_Player.Character.Humanoid.MaxHealth) * 100
            settings.FovRadius = math.clamp(dynamicSettings.MinFOV + ((100 - healthPercent) / dynamicSettings.HealthThreshold) * (dynamicSettings.MaxFOV - dynamicSettings.MinFOV), dynamicSettings.MinFOV, dynamicSettings.MaxFOV)
        end
    end

    if settings.FovShape == "Square" then
        local halfSize = settings.FovRadius / 2
        local corners = {
            V2(Mouse.X - halfSize, Mouse.Y - halfSize),
            V2(Mouse.X + halfSize, Mouse.Y - halfSize),
            V2(Mouse.X + halfSize, Mouse.Y + halfSize),
            V2(Mouse.X - halfSize, Mouse.Y + halfSize)
        }
        for i = 1, 4 do
            local line = Drawing.new("Line")
            line.Visible = settings.FovVisible
            line.From = corners[i]
            line.To = corners[i % 4 + 1]
            line.Color = settings.FovColor
            line.Thickness = settings.FovThickness
            line.Transparency = settings.FovTransparency
            table.insert(FovParts, line)
        end
    elseif settings.FovShape == "Triangle" then
        local points = {
            V2(Mouse.X, Mouse.Y - settings.FovRadius),
            V2(Mouse.X + settings.FovRadius * math.sin(math.rad(60)), Mouse.Y + settings.FovRadius * math.cos(math.rad(60))),
            V2(Mouse.X - settings.FovRadius * math.sin(math.rad(60)), Mouse.Y + settings.FovRadius * math.cos(math.rad(60)))
        }
        for i = 1, 3 do
            local line = Drawing.new("Line")
            line.Visible = settings.FovVisible
            line.From = points[i]
            line.To = points[i % 3 + 1]
            line.Color = settings.FovColor
            line.Thickness = settings.FovThickness
            line.Transparency = settings.FovTransparency
            table.insert(FovParts, line)
        end
    else 
        Fov.Visible = settings.FovVisible
        Fov.Radius = settings.FovRadius
        Fov.Position = V2(Mouse.X, Mouse.Y + (G:GetService("GuiService"):GetGuiInset().Y))
        Fov.Color = settings.FovColor
        Fov.Thickness = settings.FovThickness
        Fov.Transparency = settings.FovTransparency
        Fov.Filled = settings.Filled
        if settings.Filled then
            Fov.Transparency = settings.FillTransparency
        end
    end
end

-- // Notification Function // -- made by togglecat576 ://
local function sendNotification(title, text, icon)
    if not getgenv().Togglecat576Silent.ToggleNotification then
        StarterGui:SetCore("SendNotification", {
            Title = title,
            Text = text,
            Icon = icon,
            Duration = 5
        })
    end
end

-- // Watermark Function // -- made by togglecat576 ://
local function showWatermark()
    if getgenv().Togglecat576Silent.Watermark.Enabled then
        local screenGui = Instance.new("ScreenGui", game.CoreGui)
        local frame = Instance.new("Frame", screenGui)
        frame.Size = UDim2.new(0, 400, 0, 150)  -- Increased size for better visibility
        frame.Position = UDim2.new(0.5, -200, 0.5, -75)  -- Centered on screen
        frame.BackgroundTransparency = 1
        frame.AnchorPoint = Vector2.new(0.5, 0.5)
        frame.ClipsDescendants = true
        frame.ZIndex = 2

        local title = Instance.new("TextLabel", frame)
        title.Size = UDim2.new(1, 0, 0.6, 0)
        title.Position = UDim2.new(0, 0, 0.1, 0)
        title.BackgroundTransparency = 1
        title.TextColor3 = Color3.fromRGB(255, 20 ,147)
        title.Text = "576"
        title.Font = Enum.Font.GothamBlack
        title.TextScaled = true
        title.TextWrapped = true
        title.ZIndex = 3

        local subtitle = Instance.new("TextLabel", frame)
        subtitle.Size = UDim2.new(1, 0, 0.3, 0)
        subtitle.Position = UDim2.new(0, 0, 0.7, 0)
        subtitle.BackgroundTransparency = 1
        subtitle.TextColor3 = Color3.fromRGB(255, 20 ,147)
        subtitle.Text = "made by togglecat576"
        subtitle.Font = Enum.Font.Gotham
        subtitle.TextScaled = true
        subtitle.TextWrapped = true
        subtitle.ZIndex = 3

        frame.BackgroundTransparency = 1
        frame:TweenSizeAndPosition(
            UDim2.new(0, 400, 0, 150),
            UDim2.new(0.5, -200, 0.5, -75),
            Enum.EasingDirection.Out,
            Enum.EasingStyle.Quad,
            0.5,
            true
        )

        delay(7, function()
            frame:TweenSizeAndPosition(
                UDim2.new(0, 0, 0, 0),
                UDim2.new(0.5, 0, 0.5, 0),
                Enum.EasingDirection.In,
                Enum.EasingStyle.Quad,
                0.5,
                true,
                function()
                    screenGui:Destroy()
                end
            )
        end)
        
        if getgenv().Togglecat576.Watermark.Sound then
            local sound = Instance.new("Sound", SoundService)
            sound.SoundId = "rbxassetid://9119802009"
            sound:Play()
            delay(3, function()
                sound:Destroy()
            end)
        end

        if getgenv().Togglecat576Silent.Watermark.Music then
            local music = Instance.new("Sound", SoundService)
            music.SoundId = "rbxassetid://9045766377"
            music.Looped = true
            music:Play()
        end
    end
end

-- // Knock Check // -- made by togglecat576 ://
local function Death(Plr)
    if Plr.Character and Plr.Character:FindFirstChild("BodyEffects") then
        local bodyEffects = Plr.Character.BodyEffects
        local ko = bodyEffects:FindFirstChild("K.O") or bodyEffects:FindFirstChild("KO")
        return ko and ko.Value
    end
    return false
end

-- // Grab Check // -- made by togglecat576 ://
local function Grabbed(Plr)
    return Plr.Character and Plr.Character:FindFirstChild("GRABBING_CONSTRAINT") ~= nil
end

-- // Fall Check // -- made by togglecat576 ://
local function isFalling(Plr)
    if Plr.Character and Plr.Character:FindFirstChild("Humanoid") then
        return Plr.Character.Humanoid:GetState() == Enum.HumanoidStateType.Freefall
    end
    return false
end

-- // Check if Part in Fov and Visible // -- made by togglecat576 ://
local function isPartInFovAndVisible(part)
    local screenPoint, onScreen = Current_Camera:WorldToScreenPoint(part.Position)
    local distance = (V2(screenPoint.X, screenPoint.Y) - V2(Mouse.X, Mouse.Y)).Magnitude
    return onScreen and distance <= getgenv().Togglecat576Silent.FovSettings.FovRadius
end

-- // Check if Part Visible // -- made by togglecat576 ://
local function isPartVisible(part)
    if not getgenv().Togglecat576Silent.WallCheck then 
        return true
    end
    local origin = Current_Camera.CFrame.Position
    local direction = (part.Position - origin).Unit * (part.Position - origin).Magnitude
    local ray = Ray.new(origin, direction)
    local hit = Workspace:FindPartOnRayWithIgnoreList(ray, {Local_Player.Character, part.Parent})
    return hit == part or not hit
end

-- // Get Closest Hit Point on Part // -- made by togglecat576 ://
local function GetClosestHitPoint(character)
    local closestPart = nil
    local closestPoint = nil
    local shortestDistance = math.huge

    for _, part in pairs(character:GetChildren()) do
        if part:IsA("BasePart") and isPartInFovAndVisible(part) and isPartVisible(part) then
            local partSize = part.Size
            local halfSize = partSize / 2
            local partCorners = {
                part.CFrame * Vector3.new(-halfSize.X * 0.7, -halfSize.Y * 0.7, -halfSize.Z * 0.7),
                part.CFrame * Vector3.new(halfSize.X * 0.7, -halfSize.Y * 0.7, -halfSize.Z * 0.7),
                part.CFrame * Vector3.new(halfSize.X * 0.7, halfSize.Y * 0.7, -halfSize.Z * 0.7),
                part.CFrame * Vector3.new(-halfSize.X * 0.7, halfSize.Y * 0.7, -halfSize.Z * 0.7),
                part.CFrame * Vector3.new(-halfSize.X * 0.7, -halfSize.Y * 0.7, halfSize.Z * 0.7),
                part.CFrame * Vector3.new(halfSize.X * 0.7, -halfSize.Y * 0.7, halfSize.Z * 0.7),
                part.CFrame * Vector3.new(halfSize.X * 0.7, halfSize.Y * 0.7, halfSize.Z * 0.7),
                part.CFrame * Vector3.new(-halfSize.X * 0.7, halfSize.Y * 0.7, halfSize.Z * 0.7)
            }

            for _, corner in pairs(partCorners) do
                local screenPoint, onScreen = Current_Camera:WorldToScreenPoint(corner)
                local distance = (V2(screenPoint.X, screenPoint.Y) - V2(Mouse.X, Mouse.Y)).Magnitude

                if onScreen and distance < shortestDistance then
                    closestPart = part
                    closestPoint = corner
                    shortestDistance = distance
                end
            end
        end
    end

    return closestPart, closestPoint
end

-- // Get Closest Hit Point on Base Part // -- made by togglecat576 ://
local function GetClosestBasePart(character)
    local closestPart = nil
    local shortestDistance = math.huge
    local baseParts = {
        "Head", "HumanoidRootPart", "LeftLeg", "RightLeg",
        "LeftArm", "RightArm", "UpperTorso", "LowerTorso",
        "LeftLowerLeg", "RightLowerLeg"
    }

    for _, partName in pairs(baseParts) do
        local part = character:FindFirstChild(partName)
        if part and isPartInFovAndVisible(part) and isPartVisible(part) then
            local screenPoint, onScreen = Current_Camera:WorldToScreenPoint(part.Position)
            local distance = (V2(screenPoint.X, screenPoint.Y) - V2(Mouse.X, Mouse.Y)).Magnitude

            if onScreen and distance < shortestDistance then
                closestPart = part
                shortestDistance = distance
            end
        end
    end

    return closestPart, closestPart and closestPart.Position or nil
end

-- // Get Velocity Function // -- made by togglecat576 ://
local function GetVelocity(player, part)
    if player and player.Character then
        local velocity = player.Character[part].Velocity
        if velocity.Y < -30 and getgenv().Togglecat576Silent.Prediction.Resolver then
            return Vector3.new(velocity.X, 0, velocity.Z)
        elseif velocity.Magnitude > 50 and getgenv().Togglecat576Silent.Prediction.Resolver then
            return player.Character:FindFirstChild("Humanoid").MoveDirection * 16
        else
            return velocity
        end
    end
    return Vector3.new(0, 0, 0)
end

-- // Get Target Hit Point // -- made by togglecat576 ://
local function GetTargetHitPoint(character)
    local hitPart = character:FindFirstChild(getgenv().Togglecat576Silent.Targeting.HitPart)
    if hitPart and isPartVisible(hitPart) and isPartInFovAndVisible(hitPart) then
        return hitPart, hitPart.Position
    end
    return nil, nil
end

-- // Get Closest Player // -- made by togglecat576 ://
local function GetClosestPlr()
    local closestTarget = nil
    local maxDistance = math.huge

    for _, player in pairs(Players:GetPlayers()) do
        if player.Character and player ~= Local_Player and not Death(player) and not Grabbed(player) then
            local closestPart, closestPoint
            if getgenv().Togglecat576Silent.Targeting.TargetMode == "Closest" then
                if getgenv().Togglecat576Silent.Targeting.ClosestHitPoint then
                    closestPart, closestPoint = GetClosestHitPoint(player.Character)
                else
                    closestPart, closestPoint = GetClosestBasePart(player.Character)
                end
            else
                closestPart, closestPoint = GetTargetHitPoint(player.Character)
            end

            if closestPart and closestPoint and isPartVisible(closestPart) then
                local screenPoint = Current_Camera:WorldToScreenPoint(closestPoint)
                local distance = (V2(screenPoint.X, screenPoint.Y) - V2(Mouse.X, Mouse.Y)).Magnitude
                if distance < maxDistance then
                    maxDistance = distance
                    closestTarget = player
                end
            end
        end
    end

    return closestTarget
end

-- // Toggle Feature // -- made by togglecat576 ://
local function toggleFeature()
    getgenv().Togglecat576Silent.Enabled = not getgenv().Togglecat576Silent.Enabled
    local status = getgenv().Togglecat576Silent.Enabled and "Togglecat576's slient Enabled" or "Togglecat576's silent Disabled"
    if not getgenv().Togglecat576Silent.ToggleNotification then
        sendNotification("Togglecat576's silent", status, "rbxassetid://17561420493")
    end
    if not getgenv().Togglecat576Silent.Enabled then
        Fov.Visible = false
        HitPoint.Visible = false
        clearFovParts()
    end
end

-- // Convert Keybind to KeyCode // -- made by togglecat576 ://
local function getKeyCodeFromString(key)
    return Enum.KeyCode[key]
end

-- // Keybind Listener // -- made by togglecat576 ://
UserInputService.InputBegan:Connect(function(input, gameProcessed)
    if not gameProcessed and input.KeyCode == getKeyCodeFromString(getgenv().Togglecat576Silent.Keybind) then
        toggleFeature()
    end
end)

-- // Main Loop // -- made by togglecat576 ://
Run_Service.RenderStepped:Connect(function()
    frameCounter = frameCounter + 1
    if getgenv().Togglecat576Silent.Enabled then
        if getgenv().Togglecat576Silent.Humanization.Enabled and (tick() - lastSwitchTime < getgenv().Togglecat576Silent.Humanization.TargetSwitchDuration) then
            return
        end

        Target = GetClosestPlr()
        if Target and Target.Character and getgenv().Togglecat576Silent.Targeting.UntargetOnFall then
            local isCurrentlyFalling = isFalling(Target)
            if isCurrentlyFalling then
                if not fallStartTime[Target] then
                    fallStartTime[Target] = tick()
                elseif tick() - fallStartTime[Target] > getgenv().Togglecat576Silent.Targeting.FallThreshold then
                    Target = nil
                end
            else
                fallStartTime[Target] = nil
            end
        end

        updateFov()
        if Target and Target.Character then
            local closestPart, closestPoint
            if getgenv().Togglecat576Silent.Targeting.TargetMode == "Closest" then
                if getgenv().Togglecat576Silent.Targeting.ClosestHitPoint then
                    closestPart, closestPoint = GetClosestHitPoint(Target.Character)
                else
                    closestPart, closestPoint = GetClosestBasePart(Target.Character)
                end
            else
                closestPart, closestPoint = GetTargetHitPoint(Target.Character)
            end

            if closestPart and closestPoint then
                local hitPointSettings = getgenv().Togglecat576Silent.HitPoint
                if hitPointSettings.ShowHitPoint then
                    HitPoint.Visible = true
                    local screenPoint = Current_Camera:WorldToViewportPoint(closestPoint)
                    HitPoint.Position = V2(screenPoint.X, screenPoint.Y)
                    HitPoint.Color = hitPointSettings.HitPointColor
                    HitPoint.Radius = hitPointSettings.HitPointRadius
                    HitPoint.Thickness = hitPointSettings.HitPointThickness
                    HitPoint.Transparency = hitPointSettings.HitPointTransparency
                    HitPoint.Filled = true
                else
                    HitPoint.Visible = false
                end

                local velocity = GetVelocity(Target, closestPart.Name)
                local predictionOffset = Vector3.new(
                    velocity.X * getgenv().Togglecat576Silent.Prediction.Horizontal,
                    velocity.Y * getgenv().Togglecat576Silent.Prediction.Vertical,
                    velocity.Z * getgenv().Togglecat576Silent.Prediction.Horizontal
                )
                Replicated_Storage.MainEvent:FireServer("UpdateMousePosI", closestPoint + predictionOffset)
            end

            lastSwitchTime = tick()
        else
            HitPoint.Visible = false
        end
    end
end)

-- // Hook Tool Activation // -- made by togglecat576 ://
local function HookTool(tool)
    if tool:IsA("Tool") then
        tool.Activated:Connect(function()
            if Target and Target.Character and tick() - lastToolUse > 0.1 then  -- Debounce for 0.1 seconds
                lastToolUse = tick()
                local closestPart, closestPoint
                if getgenv().Togglecat576Silent.Targeting.TargetMode == "Closest" then
                    if getgenv().Togglecat576Silent.Targeting.ClosestHitPoint then
                        closestPart, closestPoint = GetClosestHitPoint(Target.Character)
                    else
                        closestPart, closestPoint = GetClosestBasePart(Target.Character)
                    end
                else
                    closestPart, closestPoint = GetTargetHitPoint(Target.Character)
                end

                if closestPart and closestPoint then
                    local velocity = GetVelocity(Target, closestPart.Name)
                    local predictionOffset = Vector3.new(
                        velocity.X * getgenv().Togglecat576Silent.Prediction.Horizontal,
                        velocity.Y * getgenv().Togglecat576Silent.Prediction.Vertical,
                        velocity.Z * getgenv().Togglecat576Silent.Prediction.Horizontal
                    )
                    Replicated_Storage.MainEvent:FireServer("UpdateMousePosI", closestPoint + predictionOffset)
                end
            end
        end)
    end
end

local function onCharacterAdded(character)
    character.ChildAdded:Connect(HookTool)
    for _, tool in pairs(character:GetChildren()) do
        HookTool(tool)
    end
end

Local_Player.CharacterAdded:Connect(onCharacterAdded)
if Local_Player.Character then
    onCharacterAdded(Local_Player.Character)
end

-- // Initial Notification // -- made by togglecat576 ://
sendNotification("this script was made by togglecat576")
wait(3)
sendNotification("this script was made by togglecat576")

showWatermark()
