import * as fs from 'fs'

type map_str_str = Map<string, string>;

const outContent: string = fs.readFileSync("./chain_out.txt", "utf-8");
const inContent: string = fs.readFileSync("./chain_out.txt", "utf-8");
const outContentLines: string[] = outContent.trim().split('\n');
const inContentLines: string[] = inContent.trim().split('\n');

const populateMap = (lines: string[]): map_str_str => {
    let out: map_str_str = new Map<string, string>();
    lines.forEach((line: string) => {
      const components: string[] = line.trim().split(' ');
      if(components.length === 2){
        const key: string = components[0];
        const value: string = components[1];
        out.set(key, value);
      }
    });

    return out;
}

const outMap: map_str_str = populateMap(outContentLines);
const inMap: map_str_str = populateMap(inContentLines); 

// Check if the maps have the same size + the value of one map corresponds to otherMap[thisMap.key]
const areMapsEqualAsync = (inMap: map_str_str, 
    outMap: map_str_str): Promise<boolean> => {
        return new Promise<boolean>((resolve, reject) => {
            if(inMap.size !== outMap.size)
                reject(new Error("Maps are of different size\n"));
            
            let sameKeyVal: number = 0;
            inMap.forEach((value: string, key: string) => {
                if(value !== outMap.get(key)){
                    reject(new Error("Maps hash to different keys\n"));
                }
                else sameKeyVal++;
            });

            if(sameKeyVal === inMap.size)
                resolve(true);
        });
    };

const removeElementByElementAsync = (inMap: map_str_str,
    outMap: map_str_str): Promise<map_str_str> => {
        if(inMap.size !== outMap.size)
            throw new Error("Maps are different sizes\n");
        
        // We remove from inMap every occurence of outMap's key
        return new Promise<map_str_str>((resolve, reject) => {
            outMap.forEach((value: string, key: string) => {
                if(outMap.get(key) === undefined)
                    reject(new Error("Maps hash to different keys\n"));
                
                    inMap.delete(key);
            });
            
            console.log(inMap);
            if(inMap.size === 0)
                resolve(inMap);
                
            else reject(new Error("inMap isn't empty after iterating through all of outMap\n"));
        });
    };

const checkMapsConditionsAsync = (inMap: map_str_str,
    outMap: map_str_str): Promise<boolean> => {
            return Promise.all([areMapsEqualAsync(inMap, outMap), removeElementByElementAsync(inMap, outMap)])
            .then(([recieved_bool, recieved_map]): boolean => {
                if(recieved_bool !== true || recieved_map.size > 0)
                    throw new Error("Maps are not the same\n");
                
                return true;
            });
    };

// Check whether the result is true or false
let res: Promise<boolean> = checkMapsConditionsAsync(inMap, outMap);
res.then((result) => {
  console.log(result);
}).catch((error) => {
  console.error('Error:', error);
});