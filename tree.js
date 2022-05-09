class Tree {
    #children = new Map();
    #parent = null;
    #id = Math.floor(Math.random() * 1000000)
    #name;

    constructor(name) {
        if (!name || typeof name != 'string' || !name.trim().length)
        {
            throw new Error("Name must be a non empty string");
        }
        this.#name = name;
    }

    get name()
    {
        return this.#name;
    }

    set name(newName)
    {
        if (!newName || typeof newName != 'string' || !newName.trim().length)
            throw new Error("new Name must be a non empty string");
        this.#name = newName;
    }

    get identifier()
    {
        return this.#id;
    }

    get children() {
        return Array.from(this.#children.values());
    }

    get parentNode() {
        return this.#parent
    }

    set parentNode(newParent) {
        if (newParent !== this.parentNode && (newParent === null || newParent instanceof Tree))
            this.#parent = newParent;
    }
    get childrenCount()
    {
        return this.#children.size;
    }

    createChildNode(name) {
        const newNode = new Tree(name);
        this.#children.set(newNode.identifier, newNode);
        newNode.parentNode = this;
        return newNode;
    }

    #getTreeString = (node, spaceCount = 0) => {
        let str = "\n"

        node.children.forEach((child) => {
            //console.log(child.name)
            str += `${" ".repeat(spaceCount)}${child.name}${this.#getTreeString(child, spaceCount + 2)}` 
        })
        return str;
    }

    print() {
        console.log(`\n${this.name}${this.#getTreeString(this, 2)}`)
    }
}


const tree = new Tree("root")
const pre = tree.createChildNode("pre");
const pre_1 = pre.createChildNode("pre_1");
const pre_2 = pre.createChildNode("pre_2");
console.log(tree.children)
tree.print()
// console.log(tree.name)
// console.log(tree.identifier)
// console.log(tree.parentNode)
// console.log(tree.childrenCount)
// console.log(tree.children)




