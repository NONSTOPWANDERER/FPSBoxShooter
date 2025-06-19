# FPSBoxShooter
This project uses the First Person Shooter template using Unreal Engine 5.6 and native C++ to create a dynamic, interactive shooting game. Colored, damageable boxes are fetched from online JSON data and spawned into the world. Each box has its own color, health, and score value. Shooting and destroying these boxes adds points to a live HUD display.

## Features Implemented:

### JSON-Driven Box Spawning
- Boxes are defined by online JSON data ([link](https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json)).

- Custom structs (FBoxType, FBoxObject, FBoxTransform) parse type and transform.

- BoxDataFetcher handles HTTP GET + JSON deserialization.

- BoxManager spawns each box as an ABoxActor with custom material and transform.

### Colored Box Actors
- Each ABoxActor has a custom dynamic material set in C++ using the JSON color field.

- When shot, box health decreases; on death, the player is awarded points.

### Shooting & Damage
- Integrated with the existing shooter system:
  - Projectile hits call ApplyDamage() on BoxActor.
  -   Health is reduced; box is destroyed when Health <= 0.

### Score HUD with UMG
- ShooterBulletCounterUI extended to include live score display.

- FPSBoxShooterPlayerController manages score and passes it to the HUD via UpdateScore().

- HUD updates in real-time when boxes are destroyed. 

## How to Test the Feature:
1. Launch the project in UE 5.6.

2. Play in Standalone or Selected Viewport mode.

3. You’ll see multiple colored boxes in the world (spawned from JSON).

4. Pickup the weapon from the ground.

5. Shoot boxes using your weapon:
   - Box takes damage per shot.
   - When destroyed, a score is awarded.

6. You'll will see that the score updates live in the bottom-right corner UI.

## Challenges Faced:

### Cast failures between C++ controller and Blueprint:

Resolved by ensuring BP_ShooterPlayerController is correctly parented to AFPSBoxShooterPlayerController.

### Dynamic material assignment:

Ensured material instance is created from base material in C++, not from editor assignment.

### HUD display bugs:

- Caused by Blueprint UI class reference being lost during reparenting.

- Fixed by re-exposing BulletCounterUIClass and assigning it in Blueprint.

### Projectile-damage communication:

Carefully ensured UGameplayStatics::ApplyDamage() properly reaches BoxActor.


##  Files & Structure

###  Core Logic
| File | Description |
|------|-------------|
| `BoxActor.cpp/.h` | Represents each box, handles color, health, score |
| `BoxDataFetcher.cpp/.h` | Fetches and parses JSON from URL |
| `BoxManager.cpp/.h` | Spawns boxes from parsed data |
| `BoxDataTypes.h` | Defines JSON struct types |

###  Gameplay Integration
| File | Description |
|------|-------------|
| `ShooterCharacter.cpp/.h` | Handles firing, weapon switching |
| `ShooterWeapon.cpp/.h` | Fires projectiles, manages cooldown |
| `ShooterProjectile.cpp/.h` | Detects collision and applies damage |
| `FPSBoxShooterPlayerController.cpp/.h` | Adds score, manages HUD widget |

### UI
| File | Description |
|------|-------------|
| `ShooterBulletCounterUI.cpp/.h` | UUserWidget class (C++) |
| `BP_ShooterBulletCounterUI` | Blueprint widget showing ammo + score |
| `BP_ShooterPlayerController` | Blueprint inheriting from `FPSBoxShooterPlayerController` |
