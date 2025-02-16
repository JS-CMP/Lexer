var complexArray = [
    {
        name: "John",
        age: 30,
        details: {
            address: {
                street: "123 Main St",
                city: "New York",
                zipCode: 10001,
            },
            isActive: true,
            hobbies: ["reading", "coding", "hiking"]
        }
    },
    {
        id: 42,
        data: [
            {
                key: "nestedValue",
                value: [1, 2, { deep: "level" }]
            },
            {
                mixedTypes: [null, undefined, true, 3.14, "string", { nested: false }]
            },
        ],
    },
    {
        flag: false,
        items: [
            {
                a: [
                    {
                        b: undefined,
                    }
                ]
            }
        ]
    }
];
