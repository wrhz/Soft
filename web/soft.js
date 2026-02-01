import GetFile from "./getFile.js";

var pyodide = null;

function getFiles(modules, path = "/") {
    let files = [];
    for (const module of modules.files)
    {
        files.push(path + module);
    }
    for (const [dir, module] of Object.entries(modules.dirs))
    {
        files.push(...getFiles(module, path + dir + "/"));
    }
    return files;
}

async function writeFile(modules) {
    const files = getFiles(modules);
    for (const file of files)
    {
        const path = "/lib/python3.12/site-packages" + file;
        const lastSlashIndex = path.lastIndexOf("/");
        const dirPath = path.substring(0, lastSlashIndex);
        
        if (!pyodide.FS.analyzePath(dirPath).exists)
        {
            pyodide.FS.mkdirTree(dirPath);
        }

        const data = await GetFile.getFileArrayBuffer(file);

        pyodide.FS.writeFile(path, new Uint8Array(data));
    }
}

(async function() {
    pyodide = await loadPyodide();

    const packages = await GetFile.getFileJson("/packages.json");

    await pyodide.loadPackage(packages);

    const modules = await GetFile.getFileJson("/modules_tree.json");

    await writeFile(modules);

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
    
    document.getElementsByTagName("title")[0].textContent = soft.title;

    console.log(soft.font_families)

    document.getElementById("loading").remove();
}) ()