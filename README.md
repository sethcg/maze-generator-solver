# Visual Studio Code, Vcpkg, and SDL3 Template

### Description:

<dl>
    <dd>Template for setting up a Visual Studio Code development environment with C, Vcpkg, and SDL3.
        <br/>
        <em><b>(Windows only)</b></em>
    </dd>
</dl>

#### Features:
- [x] VSCODE
- [x] Vcpkg
- [x] CMake
- [x] SDL3

---

### Developer Notes:

```bash

# CLONE REPOSITORY WITH VCPKG SUBMODULE
git clone --recurse-submodules https://github.com/sethcg/vscode-vcpkg-sdl3-template.git

```

#### Build Commands:

<details closed>
<summary>Show</summary>

```bash

# CONFIGURE
cmake -S. -Bbuild --preset windows

# BUILD
cmake --build build

```

</details>

<br/>

> [!WARNING]
> This template does not cover many of the possible use-cases by default.
> </br>
> And is not actively maintained, so I encourage you to clone and change the code to suite your needs.

</br>

---

> By wisdom a house is built, And by understanding it is established;