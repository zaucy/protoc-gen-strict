const path = require('path');
const fs = require('fs-extra');
const download = require('download');
const PLUGIN = require("./");

// Change these if you fork this repository
const VERSION_PREFIX = '';
const OWNER = 'zaucy';
const REPO = 'protoc-gen-strict';

const DL_PREFIX = `https://github.com/${OWNER}/${REPO}/releases/download/`;
const BIN_DIR = path.resolve(__dirname, "bin");
const EXT = process.platform === 'win32' ? '.exe' : '';

async function start() {
	await fs.ensureDir(BIN_DIR);
	const packageJsonPath = path.resolve(__dirname, 'package.json');
	const {version} = await fs.readJson(packageJsonPath);
	const execFilename = 'protoc-gen-strict-' + process.platform + EXT;

	const downloadUrl = DL_PREFIX + VERSION_PREFIX + 'v' + version + '/' + execFilename;

	console.log("Downloading", downloadUrl);
	const buffer = await download(downloadUrl).catch(err => {
		console.error(err.message);
		process.exit(1);
	});

	const pluginStream = fs.createWriteStream(PLUGIN);
	pluginStream.write(buffer);
	pluginStream.end(() => {
		fs.chmodSync(PLUGIN, '0755');
	});
}

start();
