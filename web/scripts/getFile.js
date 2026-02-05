export default class GetFile {
    static async getFileJson(url) {
        return await (await fetch(url)).json();
    }

    static async getFileArrayBuffer(url) {
        return await (await fetch(url)).arrayBuffer();
    }
}