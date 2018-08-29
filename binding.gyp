{
  "targets": [
    {
      "target_name": "ultradb",
      "sources": [ "src/ultradb.cc" ],
      "conditions": [
        ["OS==\"linux\"", {
          "cflags_cc": [ "-fpermissive" ]
        }]
      ]
    }
  ]
}
