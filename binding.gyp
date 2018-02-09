{
  "targets": [
    {
      "target_name": "ultradb",
      "sources": [ "ultradb.cc" ],
      "conditions": [
        ["OS==\"linux\"", {
          "cflags_cc": [ "-fpermissive" ]
        }]
      ]
    }
  ]
}
