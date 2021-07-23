(import ./support/path)

(declare-project
  :name "janet-bits" # XXX: be nice to automated processing
  :description "Janet bit patterns"
  :url "https://github.com/sogaiu/janet-bits"
  :repo "git+https://github.com/sogaiu/janet-bits")

(def proj-name
  "janet-bits")

(def proj-root
  (os/cwd))

(def proj-dir-name
  proj-name)

(def src-root
  (path/join proj-root proj-dir-name))

(declare-native
  :name "_bits"
  :cflags   [;default-cflags
             "-std=c99" "-Wall" "-Wextra"]
  :lflags [;default-ldflags]
  :source [(path/join src-root "_bits.c")])

(declare-source
  :source [proj-dir-name])

# XXX: to get .so file and friends into janet-bits subdirectory:
#
#      1. add extra phony "build" bit to copy .so file and friends
#         to janet-bits subdirectory
#      2. modify the manifest file to remove mention of .so file and
#         friends (via "install" phony)
#      3. remove .so file and friends from JANET_MODPATH root
#         as they would be redundant (they also live in janet-bits
#         subdirectory) (via "install" phony

# XXX: factor out the install and build bits below into a file that
#      ends up living in vendor subdirectory

# XXX: depend on build?
(phony "install" []
       # tweak manifest file
       (def m (parse (slurp (find-manifest proj-name))))
       (def paths (filter (fn [path]
                            # XXX: good enough?
                            (not (string/has-suffix? proj-dir-name
                                                     path)))
                          (m :paths)))
       # XXX: more elegant way based on paths value possible?
       (def keep-paths (filter (fn [path]
                                 # XXX: good enough?
                                 (string/has-suffix? proj-dir-name
                                                     path))
                               (m :paths)))
       # modify manifest file
       (spit (find-manifest proj-name)
             (string/format "%j"
                            (table/to-struct
                             (put (table ;(kvs m)) :paths
                                (array ;keep-paths)))))
       # the following makes sense given our "build" phony target below
       # delete because these should be within proj-dir-name
       (each path paths
         (os/rm path)))

# copy necessary portions within build directory into janet-bits dir
(phony "build" []
       # find build/<relevant> items
       (var build-paths nil)
       (each entry (in (dyn :rules) "build")
         (each item entry
           (when (string? item)
             (when (or (string/has-prefix? "build/" item)
                       (string/has-prefix? "build\\" item))
               (set build-paths entry)
               (break)))))
       # XXX: possibly just ignore?
       (assert build-paths "failed to find build items")
       # copy build/<relevant> to proj-dir-name
       (each path build-paths
         (copy path proj-dir-name)))

(phony "netrepl" []
       (os/execute
        ["janet" "-e" (string "(os/cd \"" src-root "\")"
                              "(import spork/netrepl)"
                              "(netrepl/server)")] :p))
