# VOX4U

[![GitHub release](https://img.shields.io/github/release/mik14a/VOX4U/all.svg)](https://github.com/mik14a/VOX4U/releases)

Import [MagicaVoxel](https://ephtracy.github.io/) vox format file to [Unreal
Engine 4](https://www.unrealengine.com/).

## Requirement

Unreal Engine 4 4.18 or later.

## Description

Import voxel objects to StaticMesh, DestructibleMesh or Voxel. No need to use
other digital content creation tool.

### StaticMesh

![StaticMesh](https://pbs.twimg.com/media/DUm7N_UVMAA5aw1.jpg)

Generate static mesh from voxel data, texture from vox file or default palette
and generate simple material for the mesh.

Can layout to the world.

#### Mesh optimized

![Optimized](https://pbs.twimg.com/media/DUm7PYxVwAIZzeZ.jpg)

Mesh generation use [a monotone decomposition
algorithm](https://0fps.net/2012/07/07/meshing-minecraft-part-2/).

### DestructibleMesh

![DestructibleMesh](https://pbs.twimg.com/media/CgKuBudUIAAbyAg.jpg)

Generate destructible mesh from voxel data and fracture meshes to destructible
component, texture from vox file or default palette and generate simple material
for the mesh.

Can layout and destruct to cell use PhysX not use [APEX Destruction
PhysXLab](https://developer.nvidia.com/apex-destruction-physxlab-tutorials).

### Voxel (Experimental support)

![Voxel](https://pbs.twimg.com/media/ChdPpU3UcAEaGr0.jpg)

Generate voxel asset based instanced static mesh component and auto generate
cube mesh and material to use to call per palette.

Can use middle size building blocks or rideable terrain on the world.

If no need to access to Voxal Actor. Can remove runtime module from uplugin and
packaging with out runtime module.

## Demo

First person template based
[demo](https://twitter.com/twitter/statuses/727524080082014208).

## Usage

Drag & Drop vox file to content panel or open import dialog and select
MagicaVoxel(*.vox) files.

## Install

```sh
cd {YourUnrealProject}
git clone https://github.com/mik14a/VOX4U.git ./Plugins/VOX4U
```

Or add submodule to your project.

```sh
cd {YourUnrealProject}
git submodule add https://github.com/mik14a/VOX4U.git ./Plugins/VOX4U
```

Binary relase is [Here](https://github.com/mik14a/VOX4U/releases). Download and
copy binaries to {YourUnrealProject}/Plugins/VOX4U. If create package without
c++ access. Copy to _Engine/Plugins/Runtime_ directory.

## Licence

[MIT License](https://github.com/mik14a/VOX4U/blob/master/LICENSE)
