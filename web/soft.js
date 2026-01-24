(async function() {
    const pyodide = await loadPyodide();

    pyodide.runPython(`
        
    `)

    document.getElementById("loading").remove();
}) ()