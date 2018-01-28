# VOX4U

MagicaVoxel VOX Format import plugin for UE4

---

Import [MagicaVoxel](https://voxel.codeplex.com) vox format file to [Unreal Engine 4](https://www.unrealengine.com/).

## Requirement

Unreal Engine 4.18 or later.

## Description

Import voxel objects to StaticMesh, DestructibleMesh or Voxel.
No need to use digital content creation tool.

### StaticMesh

![StaticMesh](https://pbs.twimg.com/media/DUm7N_UVMAA5aw1.jpg)

Generate static mesh from voxel data,
texture from vox file or default palette
and generate simple material for the mesh.

Can layout to the world.

#### Mesh optimized

![optimized](https://pbs.twimg.com/media/DUm7PYxVwAIZzeZ.jpg)

Mesh generation use [a monotone decomposition algorithm](https://0fps.net/2012/07/07/meshing-minecraft-part-2/).

### DestructibleMesh

![DestructibleMesh](https://pbs.twimg.com/media/CgKuBudUIAAbyAg.jpg)

Generate destructible mesh from voxel data
and fracture meshes to destructible component,
texture from vox file or default palette
and generate simple material for the mesh.

Can layout and destruct to cell use PhysX not use [APEX Destruction PhysXLab](https://developer.nvidia.com/apex-destruction-physxlab-tutorials).

### Voxel

![Voxel](https://pbs.twimg.com/media/ChdPpU3UcAEaGr0.jpg)

Generate voxel asset based instanced static mesh component
and auto generate cube mesh and material to use to call per palette.

Can use middle size building blocks or rideable terrain on the world.

## Demo

First person template based [demo](https://twitter.com/twitter/statuses/727524080082014208).

## Usage

Drag & Drop vox file to content panel or open import dialog and select MagicaVoxel(*.vox) files.

## Install

```sh
git clone https://github.com/mik14a/VOX4U.git ./Plugins/VOX4U
```

or

```
git submodule add https://github.com/mik14a/VOX4U.git ./Plugins/VOX4U
```

Binary relase is [Here](https://github.com/mik14a/VOX4U/releases).

## Licence

[MIT License](https://github.com/mik14a/VOX4U/blob/master/LICENSE)
