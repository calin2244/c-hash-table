"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (k !== "default" && Object.prototype.hasOwnProperty.call(mod, k)) __createBinding(result, mod, k);
    __setModuleDefault(result, mod);
    return result;
};
Object.defineProperty(exports, "__esModule", { value: true });
const fs = __importStar(require("fs"));
const outContent = fs.readFileSync("./chain_out.txt", "utf-8");
const inContent = fs.readFileSync("./chain_out.txt", "utf-8");
const outContentLines = outContent.trim().split('\n');
const inContentLines = inContent.trim().split('\n');
const populateMap = (lines) => {
    let out = new Map();
    lines.forEach((line) => {
        const components = line.trim().split(' ');
        if (components.length === 2) {
            const key = components[0];
            const value = components[1];
            out.set(key, value);
        }
    });
    return out;
};
const outMap = populateMap(outContentLines);
const inMap = populateMap(inContentLines);
// Check if the maps have the same size + the value of one map corresponds to otherMap[thisMap.key]
const areMapsEqualAsync = (inMap, outMap) => {
    return new Promise((resolve, reject) => {
        if (inMap.size !== outMap.size)
            reject(new Error("Maps are of different size\n"));
        let sameKeyVal = 0;
        inMap.forEach((value, key) => {
            if (value !== outMap.get(key)) {
                reject(new Error("Maps hash to different keys\n"));
            }
            else
                sameKeyVal++;
        });
        if (sameKeyVal === inMap.size)
            resolve(true);
    });
};
const removeElementByElementAsync = (inMap, outMap) => {
    if (inMap.size !== outMap.size)
        throw new Error("Maps are different sizes\n");
    // We remove from inMap every occurence of outMap's key
    return new Promise((resolve, reject) => {
        outMap.forEach((value, key) => {
            if (outMap.get(key) === undefined)
                reject(new Error("Maps hash to different keys\n"));
            inMap.delete(key);
        });
        console.log(inMap);
        if (inMap.size === 0)
            resolve(inMap);
        else
            reject(new Error("inMap isn't empty after iterating through all of outMap\n"));
    });
};
const checkMapsConditionsAsync = (inMap, outMap) => {
    return Promise.all([areMapsEqualAsync(inMap, outMap), removeElementByElementAsync(inMap, outMap)])
        .then(([recieved_bool, recieved_map]) => {
        if (recieved_bool !== true || recieved_map.size > 0)
            throw new Error("Maps are not the same\n");
        return true;
    });
};
// Check whether the result is true or false
let res = checkMapsConditionsAsync(inMap, outMap);
res.then((result) => {
    console.log(result);
}).catch((error) => {
    console.error('Error:', error);
});
//# sourceMappingURL=chain_checker.js.map