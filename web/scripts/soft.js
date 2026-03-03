import GetFile from "./getFile.js";

var pyodide = await loadPyodide();

async function unzip(zip_path, extract_path) {
    pyodide.FS.writeFile(zip_path, new Uint8Array(await GetFile.getFileArrayBuffer(zip_path)));

    await pyodide.runPythonAsync(`
        import shutil

        shutil.unpack_archive("${zip_path}", "/lib/python3.12/site-packages/${extract_path}")
    `);
}

function handleStyles(element, styles) {
    element.style.margin = "5";

    Object.keys(styles).forEach((key) => {
        const value = styles[key];

        switch (key) {
            case "display":
                element.style.display = value;
                break;
            case "flex_direction":
                element.style.flexDirection = value;
                break;
            case "flex_grow":
                element.style.flexGrow = value;
                break;
            case "flex_wrap":
                element.style.flexWrap = value;
                break;
            case "justify_content":
                element.style.justifyContent = value;
                break;
            case "align_items":
                element.style.alignItems = value;
                break;
            case "margin":
                element.style.margin = value;
                break;
            case "margin_top":
                element.style.marginTop = value;
                break;
            case "margin_right":
                element.style.marginRight = value;
                break;
            case "margin_bottom":
                element.style.marginBottom = value;
                break;
            case "margin_left":
                element.style.marginLeft = value;
            case "padding":
                element.style.padding = value;
                break;
            case "padding_top":
                element.style.paddingTop = value;
                break;
            case "padding_right":
                element.style.paddingRight = value;
                break;
            case "padding_bottom":
                element.style.paddingBottom = value;
                break;
            case "padding_left":
                element.style.paddingLeft = value;
            case _:
                console.error(`Unknown style: ${key}`);
        }
    });
}

function createElements(elementMap, parentElement = document.body) {
    let element;

    switch (elementMap.tag) {
        case "text":
            element = document.createElement("p");
            element.textContent = elementMap.text;
            break;
        case "view":
            element = document.createElement("div");
            break;
        default:
            throw new Error(`Unknown tag: ${elementMap.tag}`);
    }

    handleStyles(element, elementMap.style);

    elementMap.children.forEach((child) => {
        createElements(child, element);
    });

    parentElement.appendChild(element);
}

(async function() {
    const soft_config = await GetFile.getFileJson("/config/soft.json");

    const packages = await GetFile.getFileJson("/packages.json");

    await pyodide.loadPackage("micropip");

    await pyodide.runPythonAsync(`
        import micropip

        for package in ${JSON.stringify(packages)}:
            await micropip.install(package);
    `);

    const initExtensionSo = await GetFile.getFileArrayBuffer("/init_extension.so");

    pyodide.FS.writeFile("/lib/python3.12/init_extension.so", new Uint8Array(initExtensionSo));

    await pyodide.runPythonAsync(`
        import init_extension

        init_extension.init()
    `);

    await unzip("/src.zip", "src");
    await unzip("/lib.zip", "lib");
    await pyodide.runPythonAsync(`
        import sys
        import os

        sys.path.append("/lib/python3.12/site-packages/src")
        sys.path.append("/lib/python3.12/site-packages/lib")

        import lib.main as main

        soft = main.main()
    `);

    const soft = pyodide.globals.get("soft");

    const element = pyodide.globals.get("soft").home;

    document.getElementById("loading")?.remove();

    document.getElementsByTagName("title")[0].textContent = soft_config.title;

    const globalStyle = document.getElementById("global-style");

    const defaut_font_family = soft.default_font_family;

    const font_config = await GetFile.getFileJson("/config/font.json");

    Object.keys(font_config.font_families).forEach((font_family) => {
        globalStyle.textContent += `

        @font-face {
            font-family: "${font_family}";
            src: url("/resources/fonts/${font_config.font_families[font_family]}");
            font-weight: 100 | 200 | 300 | 400 | 500 | 600 | 700 | 800 | 900;
            font-style: normal;
            font-display: swap;
        }

        `
    });

    globalStyle.textContent += `
        * {
            margin: 5;
            padding: 0;
            font-family: "${defaut_font_family}";
            font-weight: bold;
            font-size: ${font_config.font_size}px;
        }
    `

    createElements(element().element.toJs({
        dict_converter: Object.fromEntries,
        create_proxies: false
    }));

    await pyodide.runPythonAsync(`
        import init_extension

        init.uninit()
    `);
}) ()