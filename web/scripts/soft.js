import GetFile from "./getFile.js";

var pyodide = await loadPyodide();
const view = document.getElementById("view");

async function unzip(zip_path, extract_path) {
    pyodide.FS.writeFile(zip_path, new Uint8Array(await GetFile.getFileArrayBuffer(zip_path)));

    await pyodide.runPythonAsync(`
        import shutil

        shutil.unpack_archive("${zip_path}", "/lib/python3.12/site-packages/${extract_path}")
    `);
}

function handleStyles(element, styles) {
    Object.keys(styles).forEach((key) => {
        const value = styles[key];

        switch (key) {
            case "horizontal_align":
                if (value === "right") {
                    element.style.justifyContent = "flex-end";
                } else if (value === "left") {
                    element.style.justifyContent = "flex-start";
                } else {
                    element.style.justifyContent = "center";
                }
                break;

            case "vertical_align":
                if (value === "top") {
                    element.style.alignItems = "flex-start";
                } else if (value === "bottom") {
                    element.style.alignItems = "flex-end";
                } else {
                    element.style.alignItems = "center";
                }
                break;
        }
    });
}

function createElements(elementMap) {
    let element;

    switch (elementMap.tag) {
        case "text":
            element = document.createElement("p");
            element.textContent = elementMap.text;
            break;
        default:
            throw new Error(`Unknown tag: ${elementMap.tag}`);
    }

    handleStyles(element, elementMap.style);

    view.appendChild(element);
}

(async function() {
    const packages = await GetFile.getFileJson("/packages.json");

    await pyodide.loadPackage(packages);

    await unzip("/src.zip", "src");
    await unzip("/lib.zip", "lib");

    await pyodide.runPythonAsync(`
        import sys

        sys.path.append("/lib/python3.12/site-packages/src")
        sys.path.append("/lib/python3.12/site-packages/lib")
    `);

    await pyodide.runPythonAsync(`
        import lib.main as main

        soft = main.main()
    `);

    const soft = pyodide.globals.get("soft");

    const element = pyodide.globals.get("soft").home().element.toJs({
        dict_converter: Object.fromEntries,
        create_proxies: false
    });

    document.getElementById("loading")?.remove();

    document.getElementsByTagName("title")[0].textContent = soft.title;

    const globalStyle = document.getElementById("global-style");

    const defaut_font_family = soft.font_family;

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
            margin: 0;
            padding: 0;
            width: 100vw;
            height: 100vh;
            min-height: 100vh;
            font-family: "${defaut_font_family}";
            font-weight: bold;
            font-size: ${font_config.font_size}px;
        }
    `

    createElements(element);
}) ()